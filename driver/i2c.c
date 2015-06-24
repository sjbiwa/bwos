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

#define	MAX_BLOCK_LENGTH		(4*8)

/* NAKチェックモード */
#define	NAK_CHECK
#if defined(NAK_CHECK)
#define	NAK_CHECK_BIT			(INT_NAKRCVIEN)
#define	NAK_CHECK_MODE			(CON_ACT2NAK_STOP)
#else
#define	NAK_CHECK_BIT			(0)
#define	NAK_CHECK_MODE			(CON_ACT2NAK_IGNORED)
#endif

typedef	enum {
	I2C_OK = 0,
	I2C_ERR,
	I2C_FATAL,
	I2C_NAK,
} I2C_RT_CODE;


typedef	struct {
	I2cDeviceInfo*	dev;					/* デバイス情報 */
	bool			active;					/* デバイス有効フラグ */
	int				mutex;					/* チャネル毎の排他用 */
	int				ev_flag;				/* 転送完了待ち用フラグ */
	TimeOut			tmout;					/* タイムアウト指定(transfer呼び出し時に設定) */
	I2C_RT_CODE		ret_code;				/* 処理終了詳細コード */
} I2cObject;


static I2cObject*		i2c_obj_tbl;
static uint32_t			i2c_obj_num;

static inline I2cObject* i2c_get_object(uint32_t port_no)
{
	return &i2c_obj_tbl[port_no];
}

static void i2c_irq_handler(uint32_t irqno, void* irq_info)
{
	I2cObject* obj = (I2cObject*)irq_info;
	uint32_t port = obj->dev->io_addr;
	iowrite32(port+I2C_IEN, 0); /* 割り込み禁止 */
	flag_set(obj->ev_flag, 0x0001);
}

static int i2c_irq_wait(I2cObject* i2c_obj, uint32_t int_spec, uint32_t* int_ret)
{
	int ret = RT_OK;
	I2cDeviceInfo* info = i2c_obj->dev;

	for (;;) {
		uint32_t ret_patn;
		iowrite32(info->io_addr+I2C_IEN, int_spec);
		int ret = flag_twait(i2c_obj->ev_flag, 0x0001, FLAG_OR|FLAG_CLR, &ret_patn, i2c_obj->tmout);
		uint32_t intr = ioread32(info->io_addr+I2C_IPD);
		if ( intr & int_spec ) {
			if ( int_ret ) {
				*int_ret = intr;
			}
			break;
		}
		if ( ret != RT_OK ) {
			/* flag_twaitがerrの場合はfatalとする */
			ret = RT_ERR;
			i2c_obj->ret_code = I2C_FATAL;

		}
	}
	return ret;
}

/* START 完了待ち */
static int i2c_start_condition(I2cObject* i2c_obj, bool is_repeat)
{
	I2cDeviceInfo* info = i2c_obj->dev;
	iowrite32(info->io_addr+I2C_IPD, 0xffffffff);
	irq_set_enable(i2c_obj->dev->irq, IRQ_ENABLE, CPU_SELF);
	uint32_t reg_value = CON_START_ENABLE|CON_I2C_EN;
	if ( is_repeat  ) {
		iowrite32(info->io_addr+I2C_CON, 0); /* repeatは一度I2C_CONを0にして再度STARTする模様 (Linuxコードより) */
		//reg_value |= CON_STOP_ENABLE; /* repeat startの場合はSTARTとSTOPの両方をONにすると動く模様 */
	}
	iowrite32(info->io_addr+I2C_CON, reg_value);

	/* 完了待ち */
	int ret = i2c_irq_wait(i2c_obj, INT_STARTIEN, NULL);

	iowrite32(info->io_addr+I2C_IEN, 0); /* 念のため */
	irq_set_enable(i2c_obj->dev->irq, IRQ_DISABLE, CPU_SELF);

	return ret;
}

/* STOP 完了待ち */
static int i2c_stop_condition(I2cObject* i2c_obj)
{
	I2cDeviceInfo* info = i2c_obj->dev;
	iowrite32(info->io_addr+I2C_IPD, 0xffffffff);
	irq_set_enable(i2c_obj->dev->irq, IRQ_ENABLE, CPU_SELF);
	iowrite32(info->io_addr+I2C_CON, CON_STOP_ENABLE|CON_I2C_EN);

	/* 完了待ち */
	int ret = i2c_irq_wait(i2c_obj, INT_STOPIEN, NULL);

	iowrite32(info->io_addr+I2C_IEN, 0); /* 念のため */
	irq_set_enable(i2c_obj->dev->irq, IRQ_DISABLE, CPU_SELF);

	return ret;
}

static int i2c_send_block(I2cObject* i2c_obj, uint8_t* buff, uint32_t length)
{
	int ret = RT_OK;
	I2cDeviceInfo* info = i2c_obj->dev;
	uint32_t word_data = 0;
	uint32_t index = 0;
	uint32_t write_index = 0;

	while ( index < length ) {
		word_data |= buff[index] << ((index % 4) * 8);
		index++;
		if ( ((index % 4) == 0) || (index == length) ) {
			iowrite32(info->io_addr+I2C_TXDATA0+write_index*4, word_data);
			write_index++;
			word_data = 0;
		}
	}

	iowrite32(info->io_addr+I2C_IPD, 0xffffffff);
	iowrite32(info->io_addr+I2C_MTXCNT, length);
	irq_set_enable(i2c_obj->dev->irq, IRQ_ENABLE, CPU_SELF);

	/* 完了待ち */
	uint32_t intr;
	ret = i2c_irq_wait(i2c_obj, NAK_CHECK_BIT|INT_MBTFIEN, &intr);
	if ( intr & NAK_CHECK_BIT ) {
		ret = RT_ERR;
		i2c_obj->ret_code = I2C_NAK;
	}

	iowrite32(info->io_addr+I2C_IEN, 0); /* 念のため */
	irq_set_enable(i2c_obj->dev->irq, IRQ_DISABLE, CPU_SELF);

	return ret;
}

static int i2c_send_addr(I2cObject* i2c_obj, uint32_t addr, I2cAddressBits bits)
{
	/* スレーブアドレスだけ別に送信する */
	uint8_t		addr_buff[2];
	uint32_t	length = 0;
	if ( bits == I2C_10BIT ) {
		/* 10bitアドレス */
		addr &= 0x3ff;
		addr_buff[0] = 0xf0 | ((addr & 0x300) >> 7);
		addr_buff[1] = addr & 0xff;
		length = 2;
	}
	else {
		/* 7bitアドレス */
		addr &= 0x7f;
		addr_buff[0] = addr << 1;
		length = 1;
	}
	return i2c_send_block(i2c_obj, addr_buff, length);
}

static int i2c_send(I2cObject* i2c_obj, uint32_t addr, I2cAddressBits bits, uint8_t* buff, uint32_t length)
{
	int ret = RT_OK;

	if ( length == 0 ) {
		i2c_obj->ret_code = I2C_ERR;
		ret = RT_ERR;
	}
	else {
		I2cDeviceInfo* info = i2c_obj->dev;
		iowrite32(info->io_addr+I2C_CON, NAK_CHECK_MODE|CON_I2C_MODE_TX_ONLY|CON_I2C_EN);

		/* スレーブアドレスを個別に送信する */
		ret = i2c_send_addr(i2c_obj, addr, bits);
		if ( ret == RT_OK ) {

			uint32_t block_num = ((length - 1) / MAX_BLOCK_LENGTH) + 1;
			uint32_t last_block_length = length % MAX_BLOCK_LENGTH;
			if ( last_block_length == 0 ) {
				last_block_length = MAX_BLOCK_LENGTH;
			}

			for ( uint32_t index = 0; index < block_num; index++, buff += MAX_BLOCK_LENGTH ) {
				ret = i2c_send_block(i2c_obj, buff, ((index+1) < block_num) ? MAX_BLOCK_LENGTH : last_block_length);
				if ( ret != RT_OK ) {
					break;
				}
			}
		}
	}
	return ret;
}

static int i2c_recv_block(I2cObject* i2c_obj, uint32_t addr, I2cAddressBits bits, uint8_t* buff, uint32_t length, bool is_first, bool is_last)
{
	I2cDeviceInfo* info = i2c_obj->dev;

	uint32_t con_value = NAK_CHECK_MODE;
	if ( is_first ) {
		con_value |= CON_I2C_MODE_RX_W_REG1|CON_I2C_EN;
		uint32_t rxaddr;
		if ( bits == I2C_10BIT ) {
			rxaddr = 0x03000000u | ((addr & 0xff) << 8) | (0xf0 | ((addr >> 7) & 0x6));
		}
		else {
			rxaddr = 0x01000000u | (addr << 1);
		}
		iowrite32(info->io_addr+I2C_MRXADDR, rxaddr);
		iowrite32(info->io_addr+I2C_MRXRADDR, 0);
	}
	else {
		con_value |= CON_I2C_MODE_RX_ONLY|CON_I2C_EN;
	}
	if ( is_last ) {
		con_value |= CON_ACK_NAK;
	}
	iowrite32(info->io_addr+I2C_CON, con_value);

	iowrite32(info->io_addr+I2C_IPD, 0xffffffff);
	iowrite32(info->io_addr+I2C_MRXCNT, length);
	irq_set_enable(i2c_obj->dev->irq, IRQ_ENABLE, CPU_SELF);

	/* 完了待ち */
	int ret = RT_OK;
	uint32_t intr;
	ret = i2c_irq_wait(i2c_obj, NAK_CHECK_BIT|INT_MBRFIEN, &intr);
	if ( intr & NAK_CHECK_BIT ) {
		ret = RT_ERR;
		i2c_obj->ret_code = I2C_NAK;
	}

	iowrite32(info->io_addr+I2C_IEN, 0); /* 念のため */
	irq_set_enable(i2c_obj->dev->irq, IRQ_DISABLE, CPU_SELF);

	if ( ret == RT_OK ) {
		/* 受信データコピー */
		uint32_t word_data = 0;
		uint32_t index = 0;

		while ( index < length ) {
			if ( (index % 4) == 0 ) {
				word_data = ioread32(info->io_addr+I2C_RXDATA0+(index & ~0x03u));
			}
			buff[index] = (word_data >> (index % 4) * 8) & 0xff;
			index++;
		}
	}
	return ret;
}


static int i2c_recv(I2cObject* i2c_obj, uint32_t addr, I2cAddressBits bits, uint8_t* buff, uint32_t length)
{
	int ret = RT_OK;

	if ( length == 0 ) {
		i2c_obj->ret_code = I2C_ERR;
		ret = RT_ERR;
	}
	else {
		uint32_t block_num = ((length - 1) / MAX_BLOCK_LENGTH) + 1;
		uint32_t last_block_length = length % MAX_BLOCK_LENGTH;
		if ( last_block_length == 0 ) {
			last_block_length = MAX_BLOCK_LENGTH;
		}

		for ( uint32_t index = 0; index < block_num; index++, buff += MAX_BLOCK_LENGTH ) {
			ret = i2c_recv_block(i2c_obj, addr, bits, buff, ((index+1) < block_num) ? MAX_BLOCK_LENGTH : last_block_length,
																index==0?true:false, ((index+1) < block_num)?false:true);
			if ( ret != RT_OK ) {
				break;
			}
		}
	}
	return ret;
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
	iowrite32(info->io_addr+I2C_CON, CON_I2C_EN);
	i2c_obj->active = true;
}

int i2c_transfer(uint32_t port_no, I2cTransferParam* param, TimeOut tmout)
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

	i2c_obj->tmout = tmout;
	i2c_obj->ret_code = I2C_OK;

	int ret = RT_OK;
	for ( uint32_t m_id = 0; (ret == RT_OK) && (m_id < param->method_num); m_id++ ) {
		/* START condition (2個目以降はrepeated START condition) */
		ret = i2c_start_condition(i2c_obj, (m_id == 0) ? false : true);
		if ( ret != RT_OK ) {
			break;
		}
		if ( param->method[m_id].mode == I2C_TX_MODE ) {
			/* 送信シーケンス */
			ret = i2c_send(i2c_obj, param->method[m_id].addr, param->method[m_id].bits, param->method[m_id].buff, param->method[m_id].length);
		}
		else {
			/* 受信シーケンス */
			ret = i2c_recv(i2c_obj, param->method[m_id].addr, param->method[m_id].bits, param->method[m_id].buff, param->method[m_id].length);
		}
	}

	/* STOP condition */
	if ( (ret == RT_OK) || (i2c_obj->ret_code == I2C_ERR) ) {
		/* 通常のSTOPシーケンス実行 */
		i2c_stop_condition(i2c_obj);
	}
	else if ( i2c_obj->ret_code == I2C_FATAL ) {
		/* 致命的エラーのため一度デバイス自体をdisableする */
		iowrite32(info->io_addr+I2C_CON, 0);
		iowrite32(info->io_addr+I2C_CON, CON_I2C_EN);
	}
	else {
		/* NAK受信時シーケンス */
		/* NAK受信を認識する場合は自動的にSTOPシーケンスが実行されるためなにもしない */
	}

	mutex_unlock(i2c_obj->mutex);

	return ret;
}
