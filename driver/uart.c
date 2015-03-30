/*
 * uart.c
 *
 *  Created on: 2015/02/01
 *      Author: biwa
 */

#include "bwos.h"
#include "driver/uart.h"
#include "driver/clock.h"

/* UARTレジスタ定義 */
#define	UART_RBR						(0x0000u)		/* Receive Buffer Register */
#define	UART_THR						(0x0000u)		/* Transmit Holding Register */
#define	UART_DLL						(0x0000u)		/* Divisor Latch (Low) */
#define	UART_DLH						(0x0004u)		/* Divisor Latch (High) */
#define	UART_IER						(0x0004u)		/* Interrupt Enable Register */
#define	UART_IIR						(0x0008u)		/* Interrupt Identification Register */
#define	UART_FCR						(0x0008u)		/* FIFO Control Register */
#define	UART_LCR						(0x000cu)		/* Line Control Register */
#define	UART_MCR						(0x0010u)		/* Modem Control Register */
#define	UART_LSR						(0x0014u)		/* Line Status Register */
#define	UART_MSR						(0x0018u)		/* Modem Status Register */
#define	UART_SCR						(0x001cu)		/* Scratchpad Register */
#define	UART_SRBR						(0x0030u)		/* Shadow Receive Buffer Register */
#define	UART_STHR						(0x0030u)		/* Shadow Transmit Holding Register */
#define	UART_FAR						(0x0070u)		/* FIFO Access Register */
#define	UART_TFR						(0x0074u)		/* Transmit FIFO Read */
#define	UART_RFW						(0x0078u)		/* Receive FIFO Write */
#define	UART_USR						(0x007cu)		/* UART Status Register */
#define	UART_TFL						(0x0080u)		/* Transmit FIFO Level */
#define	UART_RFL						(0x0084u)		/* Receive FIFO Level */
#define	UART_SRR						(0x0088u)		/* Software Reset Register */
#define	UART_SRTS						(0x008cu)		/* Shadow Request to Send */
#define	UART_SBCR						(0x0090u)		/* Shadow Break Control Register */
#define	UART_SDMAM						(0x0094u)		/* Shadow DMA Mode */
#define	UART_SFE						(0x0098u)		/* Shadow FIFO Enable */
#define	UART_SRT						(0x009cu)		/* Shadow RCVR Trigger */
#define	UART_STET						(0x00a0u)		/* Shadow TX Empty Trigger */
#define	UART_HTX						(0x00a4u)		/* Halt TX */
#define	UART_DMASA						(0x00a8u)		/* DMA Software Acknowledge */
#define	UART_CPR						(0x00f4u)		/* Component Parameter Register */
#define	UART_UCV						(0x00f8u)		/* UART Component Version */
#define	UART_CTR						(0x00fcu)		/* Component Type Register */


/* IER ビット定義 */
#define	IER_PROG_THRE_INT_EN			(0x1u<<7)		/* THRE割り込み */
#define	IER_MODEM_STATUS_INT_EN			(0x1u<<3)		/* MODEMステータス割り込み */
#define	IER_RECEIVE_LINE_STATUS_INT_EN	(0x1u<<2)		/* REceiverLineStatus割り込み */
#define	IER_TRANS_HOLD_EMPTY_INT_EN		(0x1u<<1)		/* TransmitHoldingRegisterEmpty */
#define	IER_RECEIVE_DATA_AVAILABLE_INT_EN	(0x1u<<0)	/* Received Data Available */

/* IIR */
#define	IIR_FIFOS_EN_MASK				(0x03u<<6)		/* FIFOs enable Identification */
#define	IIR_FIFOS_EN(n)					((n) & IIR_FIFOS_EN_MASK)
#define	IIR_FIFOS_EN_DISABLE			(0x0u<<6)
#define	IIR_FIFOS_EN_ENABLE				(0x3u<<6)
#define	IIR_INT_ID_MASK					(0x0fu<<0)
#define	IIR_INT_ID(n)					((n) & IIR_INT_ID_MASK)
#define	IIR_INT_ID_MODEM_STATUS			(0x00u)
#define	IIR_INT_ID_NO_INTERRUPT_PENDING	(0x01u)
#define	IIR_INT_ID_THR_EMPTY			(0x02u)
#define	IIR_INT_ID_RECEIVED_DATA_AVAILABLE	(0x04u)
#define	IIR_INT_ID_RECEIVER_LINE_STATUS	(0x06u)
#define	IIR_INT_ID_BUSY_DETECT			(0x07u)
#define	IIR_INT_ID_CHARACTER_TIMEOUT	(0x0Cu)

/* FCR (FIFO control) */
#define	FCR_RCVR_TRIGGER_MASK			(0x03u<<6)		/* Receive FIFO trigger mode */
#define	FCR_RCVR_TRIGGER_ONE			(0x0u<<6)
#define	FCR_RCVR_TRIGGER_1_4			(0x1u<<6)
#define	FCR_RCVR_TRIGGER_1_2			(0x2u<<6)
#define	FCR_RCVR_TRIGGER_LESS_2			(0x3u<<6)
#define	FCR_TX_EMPTY_TRIGGER_MASK		(0x03u<<4)		/* Transmit FIFO trigger mode */
#define	FCR_TX_TRIGGER_FIFO_EMPTY		(0x0u<<6)
#define	FCR_TX_TRIGGER_2				(0x1u<<6)
#define	FCR_TX_TRIGGER_1_4				(0x2u<<6)
#define	FCR_TX_TRIGGER_1_2				(0x3u<<6)
#define	FCR_DMA_MODE					(0x1u<<3)		/* DMA mode 0:mode0 a:character timeout */
#define	FCR_XMIT_FIFO_RESET				(0x1u<<2)
#define	FCR_RCVR_FIFO_RESET				(0x1u<<1)
#define	FCR_FIFO_EN						(0x1u<<0)

/* LCR (Line Control) */
#define	LCR_DIV_LAT_ACCESS				(0x1u<<7)
#define	LCR_BREAK_CTRL					(0x1u<<6)
#define	LCR_EVEN_PARITY_SEL				(0x1u<<5)
#define	LCR_PARITY_EN					(0x1u<<3)
#define	LCR_STOP_BITS_NUM				(0x1u<<2)
#define	LCR_DATA_LENGTH_SEL_MASK		(0x3u<<0)
#define	LCR_DATA_LENGTH_SEL_5			(0x0u<<0)
#define	LCR_DATA_LENGTH_SEL_6			(0x1u<<0)
#define	LCR_DATA_LENGTH_SEL_7			(0x2u<<0)
#define	LCR_DATA_LENGTH_SEL_8			(0x3u<<0)

/* MCR (Modem Control) */
#define	MCR_SIR_MODE_EN					(0x1u<<6)
#define	MCR_AUTO_FLOW_CTRL_EN			(0x1u<<5)
#define	MCR_LOOPBACK					(0x1u<<4)
#define	MCR_OUT2						(0x1u<<3)
#define	MCR_OUT1						(0x1u<<2)
#define	MCR_REQ_TO_SEND					(0x1u<<1)
#define	MCR_DATA_TERMINAL_READY			(0x1u<<0)

/* LSR (Line Status) */
#define	LSR_RECEIVER_FIFO_ERROR			(0x1u<<7)
#define	LSR_TRANS_EMPTY					(0x1u<<6)
#define	LSR_TRANS_HOLD_REG_EMPTY		(0x1u<<5)
#define	LSR_BREAK_INT					(0x1u<<4)
#define	LSR_FRAMING_ERROR				(0x1u<<3)
#define	LSR_PARITY_ERROR				(0x1u<<2)
#define	LSR_OVERRUN_ERROR				(0x1u<<1)
#define	LSR_DATA_READY					(0x1u<<0)

/* MSR (Modem Status) */
#define	MSR_DATA_CARRIOR_DETECT			(0x1u<<7)
#define	MSR_RING_INDICATOR				(0x1u<<6)
#define	MSR_DATA_SET_READY				(0x1u<<5)
#define	MSR_CLEAR_TO_SEND				(0x1u<<4)
#define	MSR_DELTA_DATA_CARRIER_DETECT	(0x1u<<3)
#define	MSR_TRAILING_EDGE_RING_INDICATOR	(0x1u<<2)
#define	MSR_DELTA_DATA_SET_READY		(0x1u<<1)
#define	MSR_DELTA_CLEAR_TO_SEND			(0x1u<<0)

/* FAR (FIFO access) */
#define	FAR_FIFO_ACCESS_TEST_EN			(0x1u<<0)

/* USR (UART Status) */
#define	USR_RECEIVE_FIFO_FULL			(0x1u<<4)
#define	USR_RECEIVE_FIFO_NOT_EMPTY		(0x1u<<3)
#define	USR_TRANS_FIFO_EMPTY			(0x1u<<2)
#define	USR_TRANS_FIFO_NOT_FULL			(0x1u<<1)
#define	USR_UART_BUSY					(0x1u<<0)

/* SRR (Software Reset) */
#define	SRR_XMIT_FIFO_RESET				(0x1u<<2)
#define	SRR_RCVR_FIFO_RESET				(0x1u<<1)
#define	SRR_UART_RESET					(0x1u<<0)

/* HTX (Halt TX) */
#define	HTX_HALT_TX_EN					(0x1u<<0)

/* DMASA (DMA Software Acknowledge) */
#define	DMASA_DMA_SOFTWARE_ACK			(0x1u<<0)


/* UART configuration info */
#define	FIFO_DEPTH						(64)	/* FIFOに格納できるサイズ */


typedef	struct {
	uint8_t*		buff;					/* リングバッファ */
	int				buff_size;				/* リングバッファサイズ */
	int				top;					/* 有効データ先頭位置 */
	int				length;					/* 有効データサイズ */
} RingBuff;

typedef	struct {
	bool			active;					/* UART 動作中 (open中) */
	RingBuff		tx;						/* 送信リングバッファ */
	int				tx_flg;					/* 送信完了待ち */
	RingBuff		rx;						/* 受信リングバッファ */
	int				rx_flg;					/* 受信待ち */
	uint8_t			tx_rx_temp_buff[FIFO_DEPTH]; /* 送信/受信時にテンポラリとして使うバッファ */
	UartDeviceInfo*	dev;
} UartObject;

static UartObject*		uart_obj_tbl;
static uint32_t			uart_obj_num;


static void uart_irq_handler(uint32_t irqno, void* info);

static int ringbuf_create(RingBuff* ring, uint32_t buff_size)
{
	ring->buff = sys_malloc(buff_size);
	if ( ring->buff == NULL ) {
		return RT_ERR;
	}
	ring->buff_size = buff_size;
	ring->top = 0;
	ring->length = 0;
	return RT_OK;
}

static void ringbuff_destroy(RingBuff* ring)
{
	if ( ring->buff ) {
		sys_free(ring->buff);
		ring->buff = NULL;
	}
}

static int ringbuff_write(RingBuff* ring, void* buff, int length)
{
	int ret = 0;
	int space_length = ring->buff_size - ring->length;
	if ( space_length < length ) {
		length = space_length;
	}

	if ( 0 < length ) {
		/* 書き込み位置所得 */
		int offset = ring->top + ring->length;
		if ( ring->buff_size <= offset ) {
			offset -= ring->buff_size;
		}
		int prolog_length = length;
		int epilog_length = 0;
		if ( ring->buff_size < (offset + length) ) {
			prolog_length = ring->buff_size - offset;
			epilog_length = length - prolog_length;
		}
		memcpy(ring->buff + ring->top, buff, prolog_length);
		if ( 0 < epilog_length ) {
			memcpy(ring->buff, (uint8_t*)buff + prolog_length, epilog_length);
		}
		ring->length += length;
		ret = length;
	}

	return ret;
}

static int ringbuff_read(RingBuff* ring, void* buff, int length)
{
	int ret = 0;
	if ( ring->length < length ) {
		length = ring->length;
	}

	if ( 0 < length ) {
		/* 読み込み位置所得 */
		int prolog_length = length;
		int epilog_length = 0;
		if ( ring->buff_size < (ring->top + length) ) {
			prolog_length = ring->buff_size - ring->top;
			epilog_length = length - prolog_length;
		}
		memcpy(buff, ring->buff + ring->top, prolog_length);
		if ( 0 < epilog_length ) {
			memcpy((uint8_t*)buff + prolog_length, ring->buff, epilog_length);
		}
		ring->length -= length;
		ring->top += length;
		if ( ring->buff_size <= ring->top ) {
			ring->top -= ring->buff_size;
		}
		ret = length;
	}

	return ret;
}

static UartObject* get_uart_object(uint32_t portno)
{
	return &uart_obj_tbl[portno];
}

static UartDeviceInfo* get_uart_device_info(uint32_t portno)
{
	return get_uart_object(portno)->dev;
}

void uart_register(UartDeviceInfo* info, uint32_t info_num)
{
	int ix;
	uart_obj_num = info_num;

	uart_obj_tbl = sys_malloc(sizeof(UartObject) * info_num);
	for ( ix=0; ix < info_num; ix++ ) {
		uart_obj_tbl[ix].active = false;
		uart_obj_tbl[ix].dev = &info[ix];
		irq_set_enable(uart_obj_tbl[ix].dev->irq, IRQ_DISABLE);
		irq_add_handler(uart_obj_tbl[ix].dev->irq, uart_irq_handler, &uart_obj_tbl[ix]);
	}
}

void uart_setConfig(uint32_t port_no, UartConfigParam* config)
{
	UartDeviceInfo* info = get_uart_device_info(port_no);
	uint8_t* port = info->io_addr;
	bool irq_enable_org = irq_get_enable(info->irq);

	/* UART割り込み禁止 */
	irq_set_enable(info->irq, IRQ_DISABLE);
	iowrite32(port+UART_IER, 0x0);

	/* UARTリセット */
	iowrite32(port+UART_SRR, SRR_XMIT_FIFO_RESET|SRR_RCVR_FIFO_RESET|SRR_UART_RESET);
	/* BUSY 待ち */
	while ( ioread32(port+UART_USR) & USR_UART_BUSY ) {
		task_tsleep(MSEC(10));
	}

	/* 設定 */
	uint32_t lcr = 0;
	/* ビット幅 */
	lcr |= (config->bits == UART_BITS_5) ? LCR_DATA_LENGTH_SEL_5 :
			(config->bits == UART_BITS_6) ? LCR_DATA_LENGTH_SEL_6:
			(config->bits == UART_BITS_7) ? LCR_DATA_LENGTH_SEL_7:
											LCR_DATA_LENGTH_SEL_8;
	/* パリティ */
	lcr |= (config->parity == UART_PARITY_EVEN) ? LCR_PARITY_EN|LCR_EVEN_PARITY_SEL:
			(config->parity == UART_PARITY_ODD) ? LCR_PARITY_EN:
													0;
	/* ストップビット */
	lcr |= (config->stop_bits == UART_STOPBIT_1) ? 0:
												LCR_STOP_BITS_NUM;

	/* LCR書き込み (DIV_LAT on) */
	iowrite32(port+UART_LCR, LCR_DIV_LAT_ACCESS);

	/* ボーレート設定 */
	uint32_t dll_value = clock_get(info->clock_src) / (config->baudrate * 16);
	iowrite32(port+UART_DLL, dll_value);
	iowrite32(port+UART_DLH, dll_value>>8);

	/* LCR書き込み */
	iowrite32(port+UART_LCR, LCR_DIV_LAT_ACCESS);


	iowrite32(port+UART_FCR, FCR_RCVR_TRIGGER_1_2|FCR_TX_TRIGGER_1_2|FCR_FIFO_EN);
	iowrite32(port+UART_MCR, MCR_AUTO_FLOW_CTRL_EN);

	iowrite32(port+UART_IER,
						IER_PROG_THRE_INT_EN|
						IER_MODEM_STATUS_INT_EN|
						IER_RECEIVE_LINE_STATUS_INT_EN|
						IER_TRANS_HOLD_EMPTY_INT_EN|
						IER_RECEIVE_DATA_AVAILABLE_INT_EN);
	if ( irq_enable_org ) {
		irq_set_enable(info->irq, IRQ_ENABLE);
	}
}

static void uart_release_resource(UartObject* uart_obj)
{
	irq_set_enable(uart_obj->dev->irq, IRQ_DISABLE);
	ringbuff_destroy(&(uart_obj->tx));
	ringbuff_destroy(&(uart_obj->rx));
}

void uart_open(uint32_t port_no, UartOpenParam* open)
{
	UartObject* uart_obj = get_uart_object(port_no);
	if ( uart_obj->active ) {
		uart_release_resource(uart_obj);
	}
	ringbuf_create(&(uart_obj->tx), open->tx_buff_size);
	ringbuf_create(&(uart_obj->rx), open->rx_buff_size);
	uart_obj->tx_flg = flag_create();
	uart_obj->rx_flg = flag_create();
	uart_obj->active = true;
	irq_set_enable(uart_obj->dev->irq, IRQ_ENABLE);
}

void uart_close(uint32_t port_no)
{
	UartObject* uart_obj = get_uart_object(port_no);
	if ( uart_obj->active ) {
		uart_release_resource(uart_obj);
		uart_obj->active = false;
	}
}

int uart_send(uint32_t port_no, void* buff, int length, TimeOut tmout)
{
	UartObject* uart_obj = get_uart_object(port_no);
	if ( !uart_obj->active ) {
		return RT_ERR;
	}
	UartDeviceInfo* info = uart_obj->dev;
	uint8_t* port = info->io_addr;

	int ret = 0;

	uint32_t psr;
	irq_save(psr);

	for (;;) {
		/* リングバッファに最大req_sizeだけ書き込み */
		int write_length = ringbuff_write(&uart_obj->tx, buff, length);
		if ( 0 < write_length ) {
			/* 1バイト以上書き込んだのでtx_empty割り込み有効 */
			ioset32(port+UART_IER, IER_PROG_THRE_INT_EN);
			ret = write_length;
			break;
		}
		else {
			if ( tmout != TMO_POLL ) {
				/* tx_buff not full待ち */
				uint32_t ret_ptn;
				int sc_ret = flag_twait(uart_obj->tx_flg, 0x0001, FLAG_OR|FLAG_CLR, &ret_ptn, tmout);
				if ( sc_ret == RT_OK ) {
					/* tx_buff not full になったので再度書き込み処理実行 */
					continue;
				}
				else {
					ret = sc_ret;
					break;
				}
			}
			else {
				ret = RT_TIMEOUT;
				break;
			}
		}
	}

	irq_restore(psr);
	return ret;
}

int uart_recv(uint32_t port_no, void* buff, int length, TimeOut tmout)
{
	UartObject* uart_obj = get_uart_object(port_no);
	if ( !uart_obj->active ) {
		return RT_ERR;
	}
	UartDeviceInfo* info = uart_obj->dev;
	uint8_t* port = info->io_addr;

	int ret = 0;

	uint32_t psr;
	irq_save(psr);

	for (;;) {
		/* リングバッファから最大req_size分読み込み */
		int read_length = ringbuff_read(&uart_obj->rx, buff, length);
		if ( 0 < read_length ) {
			ret = read_length;
			break;
		}
		else {
			if ( tmout != TMO_POLL ) {
				/* rx_buff not empty待ち */
				uint32_t ret_ptn;
				int sc_ret = flag_twait(uart_obj->rx_flg, 0x0001, FLAG_OR|FLAG_CLR, &ret_ptn, tmout);
				if ( sc_ret == RT_OK ) {
					/* rx_buff not empty になったので再度読み込み処理実行 */
					continue;
				}
				else {
					ret = sc_ret;
					break;
				}
			}
			else {
				ret = RT_TIMEOUT;
				break;
			}
		}
	}

	irq_restore(psr);
	return ret;
}

static void uart_irq_tx(UartObject* uart_obj, UartDeviceInfo* info, uint8_t* port)
{
	uint32_t tx_space = FIFO_DEPTH - (ioread32(port+UART_TFL) & 0x1f);
	tx_space = ringbuff_read(&uart_obj->tx, uart_obj->tx_rx_temp_buff, tx_space);
	if ( 0 < tx_space ) {
		int ix;
		for ( ix = 0; ix < tx_space; ix++ ) {
			iowrite32(port+UART_THR, uart_obj->tx_rx_temp_buff[ix]);
		}
	}
	else {
		/* 送信するデータがなくなったので 送信THR割り込みを禁止する */
		ioclr32(port+UART_IER, IER_PROG_THRE_INT_EN);
	}
}

static void uart_irq_rx(UartObject* uart_obj, UartDeviceInfo* info, uint8_t* port)
{
	uint32_t rx_remain = ioread32(port+UART_RFL) & 0x1f;
	if ( 0 < rx_remain ) {
		int ix;
		for ( ix = 0; ix < rx_remain; ix++ ) {
			uart_obj->tx_rx_temp_buff[ix] = ioread32(port+UART_RBR);
		}
		rx_remain -= ringbuff_write(&uart_obj->rx, uart_obj->tx_rx_temp_buff, rx_remain);

		if ( 0 < rx_remain ) {
			/* 受信バッファオーバーフロー */
		}
	}
}

static void uart_irq_handler(uint32_t irqno, void* irq_info)
{
	UartObject* uart_obj = (UartObject*)irq_info;
	UartDeviceInfo* info = uart_obj->dev;
	uint8_t* port = info->io_addr;

	uint32_t int_id = IIR_INT_ID(ioread32(port+UART_IIR));
	switch (int_id) {
	case IIR_INT_ID_THR_EMPTY: /* 送信 THR empty (送信キュー空きあり) */
		uart_irq_tx(uart_obj, info, port);
		break;

	case IIR_INT_ID_RECEIVED_DATA_AVAILABLE: /* 受信データあり */
	case IIR_INT_ID_CHARACTER_TIMEOUT: /* 受信データタイムアウト */
		uart_irq_rx(uart_obj, info, port);
		break;

	case IIR_INT_ID_MODEM_STATUS:
	case IIR_INT_ID_RECEIVER_LINE_STATUS:
	case IIR_INT_ID_BUSY_DETECT:
		break;

	case IIR_INT_ID_NO_INTERRUPT_PENDING:
		break;

	default:
		break;
	}
}

/* Rockchip RK3288 UART */
