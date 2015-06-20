/*
 * i2c.c
 *
 *  Created on: 2015/05/19
 *      Author: biwa
 */
#include "bwos.h"
#include "driver/i2c.h"
#include "driver/clock.h"

#define	I2C_CON					(0x0000u)		/* control register */
#define	I2C_CLKDIV				(0x0004u)		/* Clock divisor register */
#define	I2C_MRXADDR				(0x0008u)		/* the slave address accessed for master receive mode */
#define	I2C_MRXRADDR			(0x000cu)		/* the slave register address accessed for master recevie mode */
#define	I2C_MTXCNT				(0x0010u)		/* master transmit count */
#define	I2C_MRXCNT				(0x0014u)		/* master receive count */
#define	I2C_IEN					(0x0018u)		/* interrupt enable register */
#define	I2C_IPD					(0x001cu)		/* interrupt pending register */
#define	I2C_FCNT				(0x0020u)		/* finished count */
#define	I2C_TXDATA0				(0x0100u)		/* I2C transmit data register 0 */
#define	I2C_TXDATA1				(0x0104u)		/* I2C transmit data register 1 */
#define	I2C_TXDATA2				(0x0108u)		/* I2C transmit data register 2 */
#define	I2C_TXDATA3				(0x010cu)		/* I2C transmit data register 3 */
#define	I2C_TXDATA4				(0x0110u)		/* I2C transmit data register 4 */
#define	I2C_TXDATA5				(0x0114u)		/* I2C transmit data register 5 */
#define	I2C_TXDATA6				(0x0118u)		/* I2C transmit data register 6 */
#define	I2C_TXDATA7				(0x011cu)		/* I2C transmit data register 7 */
#define	I2C_RXDATA0				(0x0200u)		/* I2C receive data register 0 */
#define	I2C_RXDATA1				(0x0204u)		/* I2C receive data register 1 */
#define	I2C_RXDATA2				(0x0208u)		/* I2C receive data register 2 */
#define	I2C_RXDATA3				(0x020cu)		/* I2C receive data register 3 */
#define	I2C_RXDATA4				(0x0210u)		/* I2C receive data register 4 */
#define	I2C_RXDATA5				(0x0214u)		/* I2C receive data register 5 */
#define	I2C_RXDATA6				(0x0218u)		/* I2C receive data register 6 */
#define	I2C_RXDATA7				(0x021cu)		/* I2C receive data register 7 */


/* I2C_CON */
#define	CON_ACT2NAK_IGNORED		(0x0u<<6)
#define	CON_ACT2NAK_STOP		(0x1u<<6)
#define	CON_ACK_ACK				(0x0u<<5)
#define	CON_ACK_NAK				(0x1u<<5)
#define	CON_STOP_ENABLE			(0x1u<<4)
#define	CON_START_ENABLE		(0x1u<<3)
#define	CON_I2C_MODE_MASK		(0x3u<<1)
#define	CON_I2C_MODE_TX_ONLY	(0x0u<<1)
#define	CON_I2C_MODE_RX_W_REG1	(0x1u<<1)
#define	CON_I2C_MODE_RX_ONLY	(0x2u<<1)
#define	CON_I2C_MODE_RX_W_REG2	(0x3u<<1)
#define	CON_I2C_EN				(0x1u<<0)

/* MTXCNT/MRXCNT mask */
#define	MTXCNT_MASK				(0x3fu<<0)
#define	MRXCNT_MASK				(0x3fu<<0)

/* IEN/IPD */
#define	INT_NAKRCVIEN			(0x01u<<6)
#define	INT_STOPIEN				(0x01u<<5)
#define	INT_STARTIEN			(0x01u<<4)
#define	INT_MBRFIEN				(0x01u<<3)
#define	INT_MBTFIEN				(0x01u<<2)
#define	INT_BRFIEN				(0x01u<<1)
#define	INT_BTFIEN				(0x01u<<0)

typedef	struct {
	I2cDeviceInfo*	dev;					/* デバイス情報 */
	bool			active;					/* デバイス有効フラグ */
	int				mutex;					/* チャネル毎の排他用 */
	int				ev_flag;				/* 転送完了待ち用フラグ */
} I2cObject;


static I2cObject*		i2c_obj_tbl;
static uint32_t			i2c_obj_num;

static inline I2cObject* i2c_get_object(uint32_t port_no)
{
	return &i2c_obj_tbl[port_no];
}

static void i2c_irq_handler(uint32_t irqno, void* irq_info)
{

}
void i2c_register(I2cDeviceInfo* info, uint32_t info_num)
{
	int ix;
	i2c_obj_num = info_num;

	i2c_obj_tbl = sys_malloc(sizeof(I2cObject) * info_num);
	for ( ix=0; ix < info_num; ix++ ) {
		i2c_obj_tbl[ix].dev = &info[ix];
		i2c_obj_tbl[ix].active = false;
		i2c_obj_tbl[ix].mutex = mutex_create();
		i2c_obj_tbl[ix].ev_flag = flag_create();

		irq_set_enable(i2c_obj_tbl[ix].dev->irq, IRQ_DISABLE, CPU_SELF);
		irq_add_handler(i2c_obj_tbl[ix].dev->irq, i2c_irq_handler, &i2c_obj_tbl[ix]);
	}
}


int i2c_set_port_config(uint32_t port_no, I2cPortConfig* config)
{
	if ( !i2c_obj_tbl || (i2c_obj_num < port_no) ) {
		return RT_ERR;
	}

	I2cObject* i2c_obj = i2c_get_object(port_no);
	I2cDeviceInfo* info = i2c_obj->dev;

	iowrite32(info->io_addr+I2C_CON, 0); /* all disable */
	uint32_t clkdiv = ((clock_get(info->clock_src) / (config->baudrate * 8 * 2)) - 1) & 0xffff;
	iowrite32(info->io_addr+I2C_CLKDIV, (clkdiv<<16) | clkdiv);
	i2c_obj->active = true;
}

int i2c_transfer(uint32_t port_no, I2cTransferParam* param)
{
	if ( !i2c_obj_tbl || (i2c_obj_num < port_no) ) {
		return RT_ERR;
	}
	I2cObject* i2c_obj = i2c_get_object(port_no);
	I2cDeviceInfo* info = i2c_obj->dev;

	if ( !i2c_obj->active ) {
		return RT_ERR;
	}

	/* チャネル間排他 */
	mutex_lock(i2c_obj->mutex);

#if 1
label1:
	task_tsleep(MSEC(500));
	/* 送信ONLY */
	for (;;) {
		uint32_t intr;
		iowrite32(info->io_addr+I2C_IPD, 0xffffffff);
		intr = ioread32(info->io_addr+I2C_IPD);
		lprintf("START INTR=%08X\n", intr);
		iowrite32(info->io_addr+I2C_CON, CON_START_ENABLE|CON_I2C_MODE_TX_ONLY|CON_I2C_EN);
		iowrite32(info->io_addr+I2C_TXDATA0, 0x00000146);
		iowrite32(info->io_addr+I2C_MTXCNT, 2);

		for (;;) {
			intr = ioread32(info->io_addr+I2C_IPD);
			lprintf("INTR=%08X\n", intr);
			if ( intr & INT_MBTFIEN ) {
				intr = ioread32(info->io_addr+I2C_IPD);
				lprintf("last INTR=%08X\n", intr);
				break;
			}
			task_tsleep(MSEC(100));
		}
		iowrite32(info->io_addr+I2C_CON, CON_STOP_ENABLE|CON_I2C_MODE_TX_ONLY|CON_I2C_EN);
		task_tsleep(MSEC(500));
		break;
	}

	/* 送信ONLY */
	for (;;) {
		uint32_t intr;
		iowrite32(info->io_addr+I2C_IPD, 0xffffffff);
		intr = ioread32(info->io_addr+I2C_IPD);
		lprintf("START INTR=%08X\n", intr);
		iowrite32(info->io_addr+I2C_CON, CON_START_ENABLE|CON_I2C_MODE_TX_ONLY|CON_I2C_EN);
		iowrite32(info->io_addr+I2C_TXDATA0, 0x00001046);
		iowrite32(info->io_addr+I2C_MTXCNT, 2);

		for (;;) {
			intr = ioread32(info->io_addr+I2C_IPD);
			lprintf("INTR=%08X\n", intr);
			if ( intr & INT_MBTFIEN ) {
				intr = ioread32(info->io_addr+I2C_IPD);
				lprintf("last INTR=%08X\n", intr);
				break;
			}
			task_tsleep(MSEC(100));
		}
		iowrite32(info->io_addr+I2C_CON, CON_STOP_ENABLE|CON_I2C_MODE_TX_ONLY|CON_I2C_EN);
		task_tsleep(MSEC(500));
		break;
	}
#endif

#if 1
	task_tsleep(MSEC(100));
	/* 受信ONLY */
	for (;;) {
		uint32_t intr;
		iowrite32(info->io_addr+I2C_IPD, 0xffffffff);
		intr = ioread32(info->io_addr+I2C_IPD);
		lprintf("START INTR=%08X\n", intr);
		iowrite32(info->io_addr+I2C_CON, CON_ACK_NAK|CON_START_ENABLE|CON_I2C_MODE_RX_W_REG1|CON_I2C_EN);
		iowrite32(info->io_addr+I2C_MRXADDR, 0x01000046);
		iowrite32(info->io_addr+I2C_MRXRADDR, 0x00000000);
		iowrite32(info->io_addr+I2C_MRXCNT, 2);

		for (;;) {
			intr = ioread32(info->io_addr+I2C_IPD);
			lprintf("INTR=%08X\n", intr);
			if ( intr & INT_MBRFIEN ) {
				intr = ioread32(info->io_addr+I2C_IPD);
				lprintf("last INTR=%08X\n", intr);
				break;
			}
			task_tsleep(MSEC(100));
		}
		lprintf("READ=%08X\n", ioread32(info->io_addr+I2C_RXDATA0));
		iowrite32(info->io_addr+I2C_CON, CON_STOP_ENABLE|CON_I2C_MODE_RX_W_REG1|CON_I2C_EN);
		task_tsleep(SEC(1));
	}
#endif

#if 0
	/* 受信ONLY */
	for (;;) {
		uint32_t intr;
		/* CMD/REG 送信 */
		iowrite32(info->io_addr+I2C_IPD, 0xffffffff);
		intr = ioread32(info->io_addr+I2C_IPD);
		lprintf("START INTR=%08X\n", intr);
		iowrite32(info->io_addr+I2C_CON, CON_START_ENABLE|CON_I2C_MODE_TX_ONLY|CON_I2C_EN);
		iowrite32(info->io_addr+I2C_TXDATA0, 0x0000d0ee);
		iowrite32(info->io_addr+I2C_MTXCNT, 2);
		for (;;) {
			intr = ioread32(info->io_addr+I2C_IPD);
			lprintf("T1:INTR=%08X\n", intr);
			if ( intr & INT_MBTFIEN ) {
				iowrite32(info->io_addr+I2C_IPD, intr);
				break;
			}
			task_tsleep(MSEC(100));
		}

		iowrite32(info->io_addr+I2C_CON, CON_START_ENABLE|CON_I2C_MODE_TX_ONLY|CON_I2C_EN);
		iowrite32(info->io_addr+I2C_TXDATA0, 0x000000ef);
		iowrite32(info->io_addr+I2C_MTXCNT, 1);
		for (;;) {
			intr = ioread32(info->io_addr+I2C_IPD);
			lprintf("T2:INTR=%08X\n", intr);
			if ( intr & INT_MBTFIEN ) {
				iowrite32(info->io_addr+I2C_IPD, intr);
				break;
			}
			task_tsleep(MSEC(10));
		}

		iowrite32(info->io_addr+I2C_CON, CON_ACK_NAK|CON_I2C_MODE_RX_ONLY|CON_I2C_EN);
		iowrite32(info->io_addr+I2C_MRXCNT, 1);
		for (;;) {
			intr = ioread32(info->io_addr+I2C_IPD);
			lprintf("R:INTR=%08X\n", intr);
			if ( intr & INT_MBRFIEN ) {
				iowrite32(info->io_addr+I2C_IPD, 0xffffffff);
				break;
			}
			task_tsleep(MSEC(10));
		}
		lprintf("READ=%08X\n", ioread32(info->io_addr+I2C_RXDATA0));

		iowrite32(info->io_addr+I2C_CON, CON_STOP_ENABLE|CON_I2C_MODE_TX_ONLY|CON_I2C_EN);

		task_tsleep(MSEC(300));
	}
#endif

	mutex_unlock(i2c_obj->mutex);

	return RT_OK;
}
