/*
 * spi.c
 *
 *  Created on: 2015/05/02
 *      Author: biwa
 */
#include "bwos.h"
#include "driver/clock.h"
#include "driver/spi.h"
#include "cp15reg.h"

#define	SPI_CTRLR0				(0x0000u)		/* Control Register 0 */
#define	SPI_CTRLR1				(0x0004u)		/* Control Register 1 */
#define	SPI_ENR					(0x0008u)		/* SPI Enable */
#define	SPI_SER					(0x000cu)		/* Slave Enable Register */
#define	SPI_BAUDR				(0x0010u)		/* Baud Rate Select */
#define	SPI_TXFTLR				(0x0014u)		/* Transmit FIFO Threshold Level */
#define	SPI_RXFTLR				(0x0018u)		/* Receive FIFO Threshold Level */
#define	SPI_TXFLR				(0x001cu)		/* Transmit FIFO Level */
#define	SPI_RXFLR				(0x0020u)		/* Receive FIFO Level */
#define	SPI_SR					(0x0024u)		/* SPI Status */
#define	SPI_IPR					(0x0028u)		/* Interrupt Polarity */
#define	SPI_IMR					(0x002cu)		/* Interrupt Mask */
#define	SPI_ISR					(0x0030u)		/* Interrupt Status */
#define	SPI_RISR				(0x0034u)		/* Raw Interrupt Status */
#define	SPI_ICR					(0x0038u)		/* Interrupt Clear */
#define	SPI_DMACR				(0x003cu)		/* DMA Control */
#define	SPI_DMATDLR				(0x0040u)		/* DMA Transmit Data Level */
#define	SPI_DMARDLR				(0x0044u)		/* DMA Receive Data Level */
#define	SPI_TXDR				(0x0400u)		/* Transmit FIFO Data  */
#define	SPI_RXDR				(0x0800u)		/* Receive FIFO Data */


/* SPI CTRLR0 */
#define	CTRL_MTM_SEQ			(0x01u<<21)		/* Microwire Transfer Mode */
#define	CTRL_OPM_MASTER			(0x00u<<20)		/* Operation Mode */
#define	CTRL_OPM_SLAVE			(0x01u<<20)		/* Operation Mode */
#define	CTRL_XFM_MASK			(0x03u<<18)		/* Transfer Mode mask */
#define	CTRL_XFM_TXRX			(0x00u<<18)		/* Transmit & Receive */
#define	CTRL_XFM_TX				(0x01u<<18)		/* Transmit Only */
#define	CTRL_XFM_RX				(0x02u<<18)		/* Receive Only */
#define	CTRL_FRF_MASK			(0x03u<<16)		/* Frame Format mask */
#define	CTRL_FRF_SPI			(0x00u<<16)		/* Motorola SPI */
#define	CTRL_FRF_SSP			(0x01u<<16)		/* Texas Instruments SSP */
#define	CTRL_FRF_MICROWIRE		(0x02u<<16)		/* National Semiconductors Microwire */
#define	CTRL_RSD_MASK			(0x03u<<14)		/* Rxd Sample Delay */
#define	CTRL_RSD_NOT			(0x00u<<14)		/* do not delay */
#define	CTRL_RSD_1				(0x01u<<14)		/* 1 cycle delay */
#define	CTRL_RSD_2				(0x02u<<14)		/* 2 cycle delay */
#define	CTRL_RSD_3				(0x03u<<14)		/* 3 cycle delay */
#define	CTRL_BHT_APB16			(0x00u<<13)		/* APB16bit SPI8bit */
#define	CTRL_BHT_APB8			(0x01u<<13)		/* APB8bit SPI8bit */
#define	CTRL_FBM_MSB			(0x00u<<12)		/* first bit is MSB */
#define	CTRL_FBM_LSB			(0x01u<<12)		/* first bit is LSB */
#define	CTRL_EM_LITTLE			(0x00u<<11)		/* little endian */
#define	CTRL_EM_BIG				(0x01u<<11)		/* big endian */
#define	CTRL_SSD_HALF			(0x00u<<10)		/* half sclk_out cycles */
#define	CTRL_SSD_ONE			(0x01u<<10)		/* one sclk_out cycles */
#define	CTRL_CSM_MASK			(0x03u<<8)		/* Chip select mode mask */
#define	CTRL_CSM_KEEP			(0x00u<<8)		/* keep after transferred */
#define	CTRL_CSM_HALF			(0x01u<<8)		/* high half cycles after transferred */
#define	CTRL_CSM_ONE			(0x02u<<8)		/* high one cycles after transferred */
#define	CTRL_SCPOL_LOW			(0x00u<<7)		/* Inactive state of serial clock is low */
#define	CTRL_SCPOL_HIGH			(0x01u<<7)		/* Inactive state of serial clock is high */
#define	CTRL_SCPH_MIDDLE		(0x00u<<6)		/* Serial clock toggles in middle of first data bit */
#define	CTRL_SCPH_START			(0x01u<<6)		/* Serial clock toggles at start of first data bit */
#define	CTRL_CFS_MASK			(0x0Fu<<2)		/* Control Frame Size */
#define	CTRL_CFS_4				(0x03u<<2)		/* 4-bit serial data transfer */
#define	CTRL_CFS_5				(0x04u<<2)		/* 5-bit serial data transfer */
#define	CTRL_CFS_6				(0x05u<<2)		/* 6-bit serial data transfer */
#define	CTRL_CFS_7				(0x06u<<2)		/* 7-bit serial data transfer */
#define	CTRL_CFS_8				(0x07u<<2)		/* 8-bit serial data transfer */
#define	CTRL_CFS_9				(0x08u<<2)		/* 9-bit serial data transfer */
#define	CTRL_CFS_10				(0x09u<<2)		/* 10-bit serial data transfer */
#define	CTRL_CFS_11				(0x0Au<<2)		/* 11-bit serial data transfer */
#define	CTRL_CFS_12				(0x0Bu<<2)		/* 12-bit serial data transfer */
#define	CTRL_CFS_13				(0x0Cu<<2)		/* 13-bit serial data transfer */
#define	CTRL_CFS_14				(0x0Du<<2)		/* 14-bit serial data transfer */
#define	CTRL_CFS_15				(0x0Eu<<2)		/* 15-bit serial data transfer */
#define	CTRL_CFS_16				(0x0Fu<<2)		/* 16-bit serial data transfer */
#define	CTRL_DFS_MASK			(0x03u<<0)		/* Data Frame Size mask */
#define	CTRL_DFS_4				(0x00u<<0)		/* 4bit data */
#define	CTRL_DFS_8				(0x01u<<0)		/* 8bit data */
#define	CTRL_DFS_16				(0x02u<<0)		/* 16bit data */


#define	ENR_DISABLE				(0x00u<<0)		/* SPI disable */
#define	ENR_ENABLE				(0x01u<<0)		/* SPI enable */

#define	SER_ENABLE0				(0x01u<<0)		/* Slave Select Enable 0 */
#define	SER_ENABLE1				(0x01u<<1)		/* Slave Select Enable 1 */

#define	FIFO_THRESHOLD_MASK		(0x1Fu)			/* FIFO Threshold mask */
#define	FIFO_LEVEL_MASK			(0x3Fu)			/* FIFO Level mask */

/* SPI SR */
#define	SR_RFF					(0x01u<<4)		/* Receive FIFO Full */
#define	SR_RFE					(0x01u<<3)		/* Receive FIFO Empty */
#define	SR_TFE					(0x01u<<2)		/* Transmit FIFO Empty */
#define	SR_TFF					(0x01u<<1)		/* Transmit FIFO Full */
#define	SR_BSF					(0x01u<<0)		/* SPI Busy Flag */

/* Interrupt */
#define	IPR_HIGH				(0x00u<<0)		/* Active Interrupt Polarity Level is HIGH */
#define	IPR_LOW					(0x01u<<0)		/* Active Interrupt Polarity Level is LOW */
/* Interrupt mask */
#define	IMR_RFFIM				(0x01u<<4)		/* Receive FIFO Full Interrupt Mask */
#define	IMR_RFOIM				(0x01u<<3)		/* Receive FIFO Overflow Interrupt Mask */
#define	IMR_RFUIM				(0x01u<<2)		/* Receive FIFO Underflow Interrupt Mask */
#define	IMR_TFOIM				(0x01u<<1)		/* Transmit FIFO Overflow Interrupt Mask */
#define	IMR_TFEIM				(0x01u<<0)		/* Transmit FIFO Empty Interrupt Mask */
/* Interrupt status */
#define	ISR_RFFIS				(0x01u<<4)		/* Receive FIFO Full Interrupt Status */
#define	ISR_RFOIS				(0x01u<<3)		/* Receive FIFO Overflow Interrupt Status */
#define	ISR_RFUIS				(0x01u<<2)		/* Receive FIFO Underflow Interrupt Status */
#define	ISR_TFOIS				(0x01u<<1)		/* Transmit FIFO Overflow Interrupt Status */
#define	ISR_TFEIS				(0x01u<<0)		/* Transmit FIFO Empty Interrupt Status */
/* Interrupt raw status */
#define	RISR_RFFRIS				(0x01u<<4)		/* Receive FIFO Full Raw Interrupt Status */
#define	RISR_RFORIS				(0x01u<<3)		/* Receive FIFO Overflow Raw Interrupt Status */
#define	RISR_RFURIS				(0x01u<<2)		/* Receive FIFO Underflow Raw Interrupt Status */
#define	RISR_TFORIS				(0x01u<<1)		/* Transmit FIFO Overflow Raw Interrupt Status */
#define	RISR_TFERIS				(0x01u<<0)		/* Transmit FIFO Empty Raw Interrupt Status */
/* Interrupt clear */
#define	ICR_CTFOI				(0x01u<<3)		/* Clear Transmit FIFO Overflow Interrupt */
#define	ICR_CRFOI				(0x01u<<2)		/* Clear Receive FIFO Overflow Interrupt */
#define	ICR_CRFUI				(0x01u<<1)		/* Clear Receive FIFO Underflow Interrupt */
#define	ICR_CCI					(0x01u<<0)		/* Clear Combined Interrupt */

/* DMA */
#define	DMACR_TDE_ENABLE		(0x01u<<1)		/* Transmit DMA Enable */
#define	DMACR_RDE_ENABLE		(0x01u<<0)		/* Receive DMA Enable */

#define	FIFO_DEPATH				(32)			/* Tx/Rx FIFO depth */

#define	CHANNEL_NUM				(2)				/* チャネル数 */
#define	FIXED_CTRLR0_VALUE		(CTRL_OPM_MASTER|CTRL_XFM_TXRX|CTRL_FRF_SPI|CTRL_RSD_NOT|CTRL_BHT_APB8| \
														CTRL_SSD_HALF|CTRL_CSM_KEEP) /* CTRLR0の設定される固定値 */
#define	DEFAULT_BAUDRATE		(100000)		/* デフォルトのボーレート */
#define	BUSY_CHECK_COUNT		(10)			/* 転送前にBUSYをチェックする回数 */
#define	BUSY_CHECK_CYCLE		(MSEC(1))		/* 転送前にBUSYをチェックする周期 */

typedef	enum {
	BIT_WIDTH_4 = 0,
	BIT_WIDTH_8,
	BIT_WIDTH_16
} BitWidthSpec;

typedef	struct {
	uint32_t			reg_ctrlr0;				/* SPI_CTRLR0設定値 */
	uint32_t			reg_baudr;				/* SPI_BAUDR設定値(ボーレート) */
	BitWidthSpec		bits;					/* ビット幅 */
} ChannelParams;

typedef	struct {
	SpiDeviceInfo*		dev;					/* デバイス情報 */
	bool				active;					/* SPI動作可能 */
	ChannelParams		ch_params[CHANNEL_NUM];	/* チャネル情報 */
	int					mutex;					/* チャネル毎の排他用 */
	int					ev_flag;				/* 処理完了用イベントフラグ */
} SpiObject;

static SpiObject*		spi_obj_tbl;
static uint32_t			spi_obj_num;

static uint32_t get_baud_value(SpiDeviceInfo* info, uint32_t baudrate)
{
	return (clock_get(info->clock_src) / baudrate) & 0xfffffffe;
}

static void spi_irq_handler(uint32_t irqno, void* irq_info)
{
	SpiObject* obj = (SpiObject*)irq_info;
	uint32_t port = obj->dev->io_addr;
	iowrite32(port+SPI_IMR, 0); /* 割り込み禁止 */
	iowrite32(port+SPI_ICR, ICR_CTFOI|ICR_CRFOI|ICR_CRFUI|ICR_CCI);
	flag_set(obj->ev_flag, 0x0001);
}

static void ope_tx_data(uint32_t port, void* buff, uint32_t length, BitWidthSpec bits)
{
	if ( bits == BIT_WIDTH_16 ) {
		uint16_t* tx_buff = buff;
		for ( ; 0 < length; --length, ++tx_buff ) {
			iowrite32(port+SPI_TXDR, *tx_buff);
		}
	}
	else {
		uint8_t* tx_buff = buff;
		for ( ; 0 < length; --length, ++tx_buff ) {
			iowrite32(port+SPI_TXDR, *tx_buff);
		}
	}
}

static void ope_tx_dummy(uint32_t port, uint32_t length, BitWidthSpec bits)
{
	for ( ; 0 < length; --length ) {
		iowrite32(port+SPI_TXDR, 0xff);
	}
}

static void ope_rx_data(uint32_t port, void* buff, uint32_t length, BitWidthSpec bits)
{
	if ( bits == BIT_WIDTH_16 ) {
		uint16_t* rx_buff = buff;
		for ( ; 0 < length; --length ) {
			*rx_buff++ = ioread32(port+SPI_RXDR);
		}
	}
	else {
		uint8_t* rx_buff = buff;
		for ( ; 0 < length; --length ) {
			*rx_buff++ = ioread32(port+SPI_RXDR);
		}
	}
}

static void ope_rx_dummy(uint32_t port, uint32_t length, BitWidthSpec bits)
{
	for ( ; 0 < length; --length ) {
		ioread32(port+SPI_RXDR);
	}
}


void spi_register(SpiDeviceInfo* info, uint32_t info_num)
{
	spi_obj_num = info_num;

	spi_obj_tbl = sys_malloc(sizeof(SpiObject) * info_num);
	for ( int ix=0; ix < info_num; ix++ ) {
		spi_obj_tbl[ix].dev = &info[ix];
		spi_obj_tbl[ix].active = false;
		spi_obj_tbl[ix].mutex = mutex_create();
		spi_obj_tbl[ix].ev_flag = flag_create();
		irq_set_enable(spi_obj_tbl[ix].dev->irq, IRQ_DISABLE, CPU_SELF);
		irq_add_handler(spi_obj_tbl[ix].dev->irq, spi_irq_handler, &spi_obj_tbl[ix]);
	}

}

int spi_set_port_config(uint32_t port_no, SpiPortConfig* config)
{
	if ( (spi_obj_num <= port_no) || spi_obj_tbl[port_no].active ) {
		return RT_ERR;
	}

	SpiObject* obj = &spi_obj_tbl[port_no];
	uint32_t port = obj->dev->io_addr;

	for ( int ch=0; ch < CHANNEL_NUM; ++ch ) {
		/* デフォルトの設定 */
		obj->ch_params[ch].reg_ctrlr0 = FIXED_CTRLR0_VALUE |
										CTRL_FBM_MSB|CTRL_EM_LITTLE|CTRL_SCPOL_LOW|CTRL_SCPH_MIDDLE|CTRL_DFS_8;
		obj->ch_params[ch].reg_baudr = get_baud_value(obj->dev, DEFAULT_BAUDRATE);
		obj->ch_params[ch].bits = BIT_WIDTH_8;
	}

	/* デバイスの初期化 */
	iowrite32(port+SPI_ENR, 0x00);
	iowrite32(port+SPI_CTRLR0, obj->ch_params[0].reg_ctrlr0); /* CH0の初期値をデバイス初期値とする */
	iowrite32(port+SPI_BAUDR, obj->ch_params[0].reg_baudr); /* CH0の初期値をデバイス初期値とする */
	iowrite32(port+SPI_TXFTLR, 0);
	iowrite32(port+SPI_RXFTLR, 0);
	iowrite32(port+SPI_SER, 0x00);

	iowrite32(port+SPI_IPR, IPR_HIGH);
	iowrite32(port+SPI_IMR, 0);	 /* 全割り込み禁止 */
	iowrite32(port+SPI_DMACR, 0);

	obj->active = true;

	return RT_OK;
}

int spi_set_channel_config(uint32_t port_no, uint32_t ch_no, SpiChannelConfig* config)
{
	/* パラメータチェック */
	if ( (spi_obj_num <= port_no) || (CHANNEL_NUM <= ch_no) || !spi_obj_tbl[port_no].active ) {
		lprintf("param error\n");
		return RT_ERR;
	}
	SpiObject* obj = &spi_obj_tbl[port_no];

	uint32_t ctrlr0 = FIXED_CTRLR0_VALUE;
	ctrlr0 |= (config->firstbit == SPI_FIRSTBIT_MSB)  ? CTRL_FBM_MSB     : CTRL_FBM_LSB;
	ctrlr0 |= (config->endian   == SPI_ENDIAN_LITTLE) ? CTRL_EM_LITTLE   : CTRL_EM_BIG;
	ctrlr0 |= (config->scpol    == SPI_SCPOL_LOW)     ? CTRL_SCPOL_LOW   : CTRL_SCPOL_HIGH;
	ctrlr0 |= (config->scph     == SPI_SCPH_MIDDLE)   ? CTRL_SCPH_MIDDLE : CTRL_SCPH_START;
	ctrlr0 |= (config->bits == 4) ? CTRL_DFS_4 : (config->bits == 8) ? CTRL_DFS_8 : CTRL_DFS_16;
	obj->ch_params[ch_no].bits = (config->bits == 4) ? BIT_WIDTH_4 : (config->bits == 8) ? BIT_WIDTH_8 : BIT_WIDTH_16;
	obj->ch_params[ch_no].reg_ctrlr0 = ctrlr0;
	obj->ch_params[ch_no].reg_baudr = get_baud_value(obj->dev, config->baudrate);
	return RT_OK;
}

int spi_transfer(uint32_t port_no, uint32_t ch_no, SpiTransferParam* param)
{
	/* パラメータチェック */
	if ( (spi_obj_num <= port_no) || (CHANNEL_NUM <= ch_no) || !spi_obj_tbl[port_no].active ) {
		lprintf("param error\n");
		return RT_ERR;
	}

	SpiObject* obj = &spi_obj_tbl[port_no];
	uint32_t port = obj->dev->io_addr;

	/* チャネル間排他 */
	mutex_lock(obj->mutex);

	iowrite32(port+SPI_ENR, 0x00); /* SPI無効化 */

	/* SPI通信設定 */
	iowrite32(port+SPI_CTRLR0, obj->ch_params[ch_no].reg_ctrlr0);
	iowrite32(port+SPI_BAUDR, obj->ch_params[ch_no].reg_baudr);
	iowrite32(port+SPI_SER, 0x01 << ch_no); /* CSn */
	/* 割り込みは禁止 / ハンドラは有効化 */
	iowrite32(port+SPI_IMR, 0);
	iowrite32(port+SPI_ICR, ICR_CTFOI|ICR_CRFOI|ICR_CRFUI|ICR_CCI);
	irq_set_enable(obj->dev->irq, IRQ_ENABLE, CPU_SELF);

	iowrite32(port+SPI_ENR, 0x01); /* SPI有効化 */

	uint32_t remain_tx_length = param->tx_length; /* 送信するデータの長さ */
	uint32_t remain_rx_length = param->rx_length; /* 受信するデータの長さ */
	void* tx_buff = param->tx_buf;
	void* rx_buff = param->rx_buf;
	uint32_t data_bytes = (obj->ch_params[ch_no].bits == BIT_WIDTH_16) ? 2 : 1; /* 1データのバイト数 */
	uint32_t remain_total_tx_length = MAX(remain_rx_length, remain_tx_length); /*送信処理(レジスタに書き込む)する長さ */
	uint32_t remain_total_rx_length = remain_total_tx_length; /* 受信処理(レジスタから読み込む)するする長さ */

	while ( (0 < remain_total_tx_length) || (0 < remain_total_rx_length) ) {
		/* 送信チェック */
		if ( 0 < remain_total_tx_length ) {
			uint32_t tfifos = (ioread32(port+SPI_TXFLR) & FIFO_LEVEL_MASK);
			if ( FIFO_DEPATH < tfifos ) {
				tfifos = FIFO_DEPATH;
			}
			uint32_t avail_tx_length = FIFO_DEPATH - tfifos; /* 送信できる最大長 */
			avail_tx_length = MIN(avail_tx_length, remain_total_tx_length);
			if ( 0 < remain_tx_length ) {
				/* 実データの送信 */
				avail_tx_length = MIN(avail_tx_length, remain_tx_length);
				ope_tx_data(port, tx_buff, avail_tx_length, obj->ch_params[ch_no].bits);
				remain_tx_length -= avail_tx_length;
				tx_buff = (uint8_t*)tx_buff + avail_tx_length * data_bytes;
			}
			else {
				/* 受信処理のためのダミー送信 */
				ope_tx_dummy(port, avail_tx_length, obj->ch_params[ch_no].bits);
			}
			remain_total_tx_length -= avail_tx_length;
		}
		/* 受信チェック */
		if ( 0 < remain_total_rx_length ) {
			uint32_t rfifos = (ioread32(port+SPI_RXFLR) & FIFO_LEVEL_MASK);
			uint32_t avail_rx_length = MIN(remain_total_rx_length, rfifos);
			if ( 0 < remain_rx_length ) {
				/* 実データの受信 */
				ope_rx_data(port, rx_buff, avail_rx_length, obj->ch_params[ch_no].bits);
				remain_rx_length -= avail_rx_length;
				rx_buff = (uint8_t*)rx_buff + avail_rx_length * data_bytes;
			}
			else {
				/* ダミー受信 */
				uint32_t avail_rx_length = MIN(remain_total_rx_length, rfifos);
				ope_rx_dummy(port, avail_rx_length, obj->ch_params[ch_no].bits);
			}
			remain_total_rx_length -= avail_rx_length;
		}

		/* イベント待ち */
		flag_clear(obj->ev_flag, 0);
		iowrite32(port+SPI_IMR, IMR_RFFIM|IMR_TFEIM); /* 受信full / 送信empty のみ */
		uint32_t ret_patn;
		flag_wait(obj->ev_flag, 0x0001, FLAG_OR|FLAG_CLR, &ret_patn);
	}

	/* 受信FIFOに残っているデータ(不要データ)をダミーリードする */
	uint32_t rfifos = (ioread32(port+SPI_RXFLR) & FIFO_LEVEL_MASK);
	ope_rx_dummy(port, rfifos, obj->ch_params[ch_no].bits);

#if 0
	/* 前回転送時のBUSY状態チェック */
	int count = BUSY_CHECK_COUNT;
	for ( ; 0 < count; --count ) {
		lprintf("SR:%08X\n", ioread32(port+SPI_SR));
		if ( (ioread32(port+SPI_SR) & SR_BSF) == 0 ) {
			break;
		}
		task_tsleep(BUSY_CHECK_CYCLE);
	}
	if ( count == 0 ) {
		/* BUSYのままなのでエラー */
		mutex_unlock(spi_obj_tbl[port_no].mutex);
		lprintf("busy error\n");
		return RT_ERR;
	}
#endif
	iowrite32(port+SPI_SER, 0x00); /* CSn OFF */
	iowrite32(port+SPI_ENR, 0x00); /* SPI無効化 */

	irq_set_enable(obj->dev->irq, IRQ_DISABLE, CPU_SELF);

	mutex_unlock(obj->mutex);

	return RT_OK;
}
