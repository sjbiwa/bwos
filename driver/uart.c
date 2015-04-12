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

/* イベント待ちフラグ */
#define	EVENT_SEND						(1u<<0)
#define	EVENT_RECV						(1u<<1)
#define	EVENT_ERROR						(1u<<2)

typedef	struct {
	uint8_t*		buff;					/* リングバッファ */
	int				buff_size;				/* リングバッファサイズ */
	int				top;					/* 有効データ先頭位置 */
	int				length;					/* 有効データサイズ */
} RingBuff;

typedef	struct {
	UartDeviceInfo*	dev;					/* デバイス情報 */
	bool			active;					/* UART 動作中 (open中) */
	int				ev_flag;				/* イベント待ちフラグ */
	uint32_t		err_bits;				/* エラービット */
	uint32_t		flow_control;			/* フロー制御 (ソフトウェア制御判定に使う) */
	RingBuff		tx;						/* 送信リングバッファ */
	RingBuff		rx;						/* 受信リングバッファ */
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

	uint32_t psr;
	irq_save(psr);

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

	irq_restore(psr);

	return ret;
}

/* ソフトウェアフロー制御時の閾値チェック */
static bool ringbuff_threshold(RingBuff* ring)
{
	bool ret = false;

	if ( 90 < ((ring->length * 100) / ring->buff_size) ) {
		ret = true;
	}

	return ret;
}

/* リニアに読み込めるバッファ領域取得 */
int ringbuff_get_linearreadable(RingBuff* ring, void** ptr)
{
	*ptr = &(ring->buff[ring->top]);
	int ret = ring->length;
	if ( ring->buff_size < (ring->top + ring->length) ) {
		ret = ring->buff_size - ring->top;
	}
	return ret;
}

/* リニアに書き込めるバッファ領域取得 */
int ringbuff_get_linearwriteable(RingBuff* ring, void** ptr)
{
	int pos = ring->top + ring->length;
	if ( ring->buff_size <= pos ) {
		pos -= ring->buff_size;
	}
	*ptr = &(ring->buff[pos]);
	int ret = ring->top - pos;
	if ( ring->top <= pos ) {
		ret = ring->buff_size - pos;
	}
	return ret;
}

/* 読み込み領域を先頭から開放 */
void ringbuff_free(RingBuff* ring, int length)
{
	if ( ring->length < length ) {
		length = ring->length;
	}
	ring->length -= length;
	ring->top += length;
	if ( ring->buff_size <= ring->top ) {
		ring->top -= ring->buff_size;
	}
}

/* 書き込み領域を末尾から確保 */
void ringbuff_alloc(RingBuff* ring, int length)
{
	ring->length += length;
	if ( ring->buff_size < ring->length ) {
		ring->length = ring->buff_size;
	}
}

/* リングバッファが一杯か判定 */
bool ringbuff_full(RingBuff* ring)
{
	bool ret = false;
	if ( ring->buff_size <= ring->length ) {
		ret = true;
	}
	return ret;
}

/* リングバッファのデータ長 */
int ringbuff_length(RingBuff* ring)
{
	return ring->length;
}

void uart_rx_reset(UartObject* uart_obj)
{
	uint8_t* port = uart_obj->dev->io_addr;
	iowrite32(port+UART_SRR,SRR_RCVR_FIFO_RESET);
}

static UartObject* uart_get_object(uint32_t portno)
{
	return &uart_obj_tbl[portno];
}

static void uart_set_rts(UartObject* uart_obj, bool flag)
{
	if ( flag ) {
		ioset32(uart_obj->dev->io_addr+UART_MCR, MCR_REQ_TO_SEND);
	}
	else {
		ioclr32(uart_obj->dev->io_addr+UART_MCR, MCR_REQ_TO_SEND);
	}
}

static bool uart_get_cts(UartObject* uart_obj)
{
	if ( ioread32(uart_obj->dev->io_addr+UART_MSR) & MSR_CLEAR_TO_SEND ) {
		return true;
	}
	return false;
}

void uart_register(UartDeviceInfo* info, uint32_t info_num)
{
	int ix;
	uart_obj_num = info_num;

	uart_obj_tbl = sys_malloc(sizeof(UartObject) * info_num);
	for ( ix=0; ix < info_num; ix++ ) {
		uart_obj_tbl[ix].active = false;
		uart_obj_tbl[ix].dev = &info[ix];
		uart_obj_tbl[ix].ev_flag = flag_create();
		uart_obj_tbl[ix].err_bits = 0;

		irq_set_enable(uart_obj_tbl[ix].dev->irq, IRQ_DISABLE);
		irq_add_handler(uart_obj_tbl[ix].dev->irq, uart_irq_handler, &uart_obj_tbl[ix]);
	}
}

void uart_set_config(uint32_t port_no, UartConfigParam* config)
{
	UartObject* uart_obj = uart_get_object(port_no);
	UartDeviceInfo* info = uart_obj->dev;
	uint8_t* port = info->io_addr;
	bool irq_enable_org = irq_get_enable(info->irq);

	/* UART割り込み禁止 */
	irq_set_enable(info->irq, IRQ_DISABLE);
	iowrite32(port+UART_IER, 0x0);

	/* UARTリセット */
	iowrite32(port+UART_SRR, SRR_XMIT_FIFO_RESET|SRR_RCVR_FIFO_RESET|SRR_UART_RESET);
	/* BUSY 待ち */
	task_tsleep(MSEC(100));
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
	iowrite32(port+UART_LCR, lcr);

	/* FIFO control (TXはempty割り込みとする) */
	iowrite32(port+UART_FCR, FCR_RCVR_TRIGGER_1_2|FCR_TX_TRIGGER_FIFO_EMPTY|FCR_FIFO_EN);

	/* フロー制御設定 */
	if ( config->flow_control == UART_FLOW_HARD ) {
		iowrite32(port+UART_MCR, MCR_AUTO_FLOW_CTRL_EN);
	}
	else {
		iowrite32(port+UART_MCR, 0);
	}
	uart_obj->flow_control = config->flow_control;

	iowrite32(port+UART_IER,
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
	UartObject* uart_obj = uart_get_object(port_no);
	if ( uart_obj->active ) {
		uart_release_resource(uart_obj);
	}
	ringbuf_create(&(uart_obj->tx), open->tx_buff_size);
	ringbuf_create(&(uart_obj->rx), open->rx_buff_size);
	/* RTS信号制御 */
	if ( uart_obj->flow_control == UART_FLOW_SOFT ) {
		uart_set_rts(uart_obj, true);
	}
	uart_obj->active = true;
	flag_clear(uart_obj->ev_flag, ~0);
	irq_set_enable(uart_obj->dev->irq, IRQ_ENABLE);
}

void uart_close(uint32_t port_no)
{
	UartObject* uart_obj = uart_get_object(port_no);
	if ( uart_obj->active ) {
		uart_release_resource(uart_obj);
		uart_obj->active = false;
	}
}

static int uart_tx_write(UartObject* uart_obj, void* buff, int length)
{
	/* CTS信号確認 */
	if ( (uart_obj->flow_control == UART_FLOW_SOFT) && !uart_get_cts(uart_obj) ) {
		/* CTSがOFFなら送信処理は行わない */
		return 0;
	}

	uint8_t* port = uart_obj->dev->io_addr;

	/* TX-FIFOに書き込めるバイト数取得 */
	uint32_t tx_length = ioread32(port+UART_TFL) & 0x7f;
	if ( FIFO_DEPTH < tx_length ) {
		tx_length = FIFO_DEPTH;
	}
	uint32_t tx_sendable = FIFO_DEPTH - tx_length;
	tx_sendable = MIN(tx_sendable, length);
	if ( 0 < tx_sendable ) {
		int ix;
		for ( ix = 0; ix < tx_sendable; ix++ ) {
			iowrite32(port+UART_THR, *(PTRVAR(buff)+ix));
		}
		flag_set(uart_obj->ev_flag, EVENT_SEND);
	}
	return tx_sendable;
}

static void uart_tx_exec(UartObject* uart_obj, uint8_t* port)
{
	for (;;) {
		bool do_loop = false;
		void* ptr;

		uint32_t psr;
		irq_save(psr);
		int length = ringbuff_get_linearreadable(&uart_obj->tx, &ptr);
		if ( 0 < length ) {
			int tx_length = uart_tx_write(uart_obj, ptr, length);
			if ( 0 < tx_length ) {
				ringbuff_free(&uart_obj->tx, tx_length);
				do_loop = true;
			}
		}
		irq_restore(psr);
		if ( !do_loop ) {
			break;
		}
	}
}

static int uart_rx_read(UartObject* uart_obj, void* buff, int length)
{
	uint8_t* port = uart_obj->dev->io_addr;
	/* RX-FIFO内のバイト数取得 */
	uint32_t rx_remain = ioread32(port+UART_RFL) & 0x7f;
	if ( FIFO_DEPTH < rx_remain ) {
		rx_remain = FIFO_DEPTH;
	}
	int rx_readable = MIN(rx_remain, length);
	if ( 0 < rx_readable ) {
		int ix;
		for ( ix = 0; ix < rx_remain; ix++ ) {
			*(PTRVAR(buff)+ix) = ioread32(port+UART_RBR);
		}
		/* RTS信号制御 */
		if ( (uart_obj->flow_control == UART_FLOW_SOFT) && ringbuff_threshold(&uart_obj->rx) ) {
			uart_set_rts(uart_obj, false);
		}

		flag_set(uart_obj->ev_flag, EVENT_RECV);
	}
	return rx_readable;
}

static void uart_rx_exec(UartObject* uart_obj, uint8_t* port)
{
	for (;;) {
		bool do_loop = false;
		void* ptr;

		uint32_t psr;
		irq_save(psr);

		int length = ringbuff_get_linearwriteable(&uart_obj->rx, &ptr);
		if ( 0 < length ) {
			int rx_length = uart_rx_read(uart_obj, ptr, length);
			if ( 0 < rx_length ) {
				ringbuff_alloc(&uart_obj->rx, rx_length);
				do_loop = true;
			}
		}
		irq_restore(psr);
		if ( !do_loop ) {
			break;
		}
	}

	if ( ringbuff_full(&uart_obj->rx) ) {
		/* 受信バッファオーバーフローとする */
		uart_rx_reset(uart_obj);
	}
}

int uart_send(uint32_t port_no, void* buff, int length, TimeOut tmout)
{
	UartObject* uart_obj = uart_get_object(port_no);
	if ( !uart_obj->active ) {
		return RT_ERR;
	}

	int ret = 0;

	for (;;) {
		int write_length = 0;

		uint32_t psr;
		irq_save(psr);
		if ( ringbuff_length(&uart_obj->tx) == 0 ) {
			/* 送信リングバッファ内が空なら直接送信する */
			write_length = uart_tx_write(uart_obj, buff, length);
			if ( write_length < length ) {
				write_length += ringbuff_write(&uart_obj->tx, PTRVAR(buff)+write_length, length-write_length);
			}
		}
		else {
			write_length = ringbuff_write(&uart_obj->tx, PTRVAR(buff), length);
		}
		irq_restore(psr);

		if ( 0 < write_length ) {
			/* 1バイト以上書き込んだので正常終了とする */
			ret = write_length;
		}
		else {
			if ( tmout != TMO_POLL ) {
				/* tx_buff not full待ち */
				uint32_t ret_ptn;
				int sc_ret = flag_twait(uart_obj->ev_flag, EVENT_SEND, FLAG_OR|FLAG_BITCLR, &ret_ptn, tmout);
				if ( sc_ret == RT_OK ) {
					/* tx_buff not full になったので再度書き込み処理実行 */
					continue; /* 処理のリトライ */
				}
				else {
					ret = sc_ret;
				}
			}
			else {
				ret = RT_TIMEOUT;
			}
		}
		/* ここまできたらループを抜ける*/
		break;
		/* */
	}

	return ret;
}

int uart_recv(uint32_t port_no, void* buff, int length, TimeOut tmout)
{
	UartObject* uart_obj = uart_get_object(port_no);
	if ( !uart_obj->active ) {
		return RT_ERR;
	}
	uint8_t* port = uart_obj->dev->io_addr;

	int ret = 0;

	for (;;) {
		/* リングバッファから最大req_size分読み込み */
		uint32_t psr;
		irq_save(psr);
		int read_length = ringbuff_read(&uart_obj->rx, buff, length);
		/* RTS信号制御 */
		if ( (uart_obj->flow_control == UART_FLOW_SOFT) && !ringbuff_threshold(&uart_obj->rx) ) {
			uart_set_rts(uart_obj, true);
		}
		irq_restore(psr);

		if ( 0 < read_length ) {
			ret = read_length;
		}
		else {
			if ( tmout != TMO_POLL ) {
				/* rx_buff not empty待ち */
				uint32_t ret_ptn;
				int sc_ret = flag_twait(uart_obj->ev_flag, EVENT_RECV, FLAG_OR|FLAG_BITCLR, &ret_ptn, tmout);
				if ( sc_ret == RT_OK ) {
					/* rx_buff not empty になったので再度読み込み処理実行 */
					continue; /* 処理のリトライ */
				}
				else {
					ret = sc_ret;
				}
			}
			else {
				ret = RT_TIMEOUT;
			}
		}
		/* ここまできたらループを抜ける*/
		break;
		/* */
	}

	return ret;
}

int uart_error(uint32_t port_no, uint32_t* err_bits, TimeOut tmout)
{
	UartObject* uart_obj = uart_get_object(port_no);
	if ( !uart_obj->active ) {
		return RT_ERR;
	}
	UartDeviceInfo* info = uart_obj->dev;
	uint8_t* port = info->io_addr;

	int ret = 0;

	for (;;) {
		if ( uart_obj->err_bits ) {
			uint32_t psr;
			irq_save(psr);
			*err_bits = uart_obj->err_bits;
			uart_obj->err_bits = 0;
			irq_restore(psr);
		}
		else {
			if ( tmout != TMO_POLL ) {
				/* error event待ち */
				uint32_t ret_ptn;
				int sc_ret = flag_twait(uart_obj->ev_flag, EVENT_ERROR, FLAG_OR|FLAG_BITCLR, &ret_ptn, tmout);
				if ( sc_ret == RT_OK ) {
					/* error eventが発生したので再度読み込み処理実行 */
					continue; /* 処理のリトライ */
				}
				else {
					ret = sc_ret;
				}
			}
			else {
				ret = RT_TIMEOUT;
			}
		}
		/* ここまできたらループを抜ける*/
		break;
		/* */
	}

	return ret;
}

static void uart_modemstatus_exec(UartObject* uart_obj, uint8_t* port)
{
	uint32_t stat = ioread32(port+UART_MSR);
	if ( stat & MSR_CLEAR_TO_SEND ) {
		/* CTS信号が変化したら送信処理を再開 */
		if ( uart_obj->flow_control == UART_FLOW_SOFT ) {
			uart_tx_exec(uart_obj, port);
		}
	}
}

static void uart_linestatus_exec(UartObject* uart_obj, uint8_t* port)
{
	uint32_t stat = ioread32(port+UART_LSR);

	if ( stat & LSR_FRAMING_ERROR ) {
		uart_obj->err_bits |= UART_ERR_FRAMING;
	}
	if ( stat & LSR_PARITY_ERROR ) {
		uart_obj->err_bits |= UART_ERR_PARITY;
	}
	if ( stat & LSR_OVERRUN_ERROR ) {
		uart_obj->err_bits |= UART_ERR_OVERRUN;
	}

	/* エラーがあればフラグセット */
	if ( uart_obj->err_bits != 0 ) {
		flag_set(uart_obj->ev_flag, EVENT_ERROR);
	}
}

static void uart_irq_handler(uint32_t irqno, void* irq_info)
{
	UartObject* uart_obj = (UartObject*)irq_info;
	uint8_t* port = uart_obj->dev->io_addr;

	uint32_t int_id = IIR_INT_ID(ioread32(port+UART_IIR));
	switch (int_id) {
	case IIR_INT_ID_THR_EMPTY: /* 送信 THR empty (送信キュー空きあり) */
		uart_tx_exec(uart_obj, port);
		break;

	case IIR_INT_ID_RECEIVED_DATA_AVAILABLE: /* 受信データあり */
	case IIR_INT_ID_CHARACTER_TIMEOUT: /* 受信データタイムアウト */
		uart_rx_exec(uart_obj, port);
		break;

	case IIR_INT_ID_MODEM_STATUS:
		uart_modemstatus_exec(uart_obj, port);
		break;

	case IIR_INT_ID_RECEIVER_LINE_STATUS:
		uart_linestatus_exec(uart_obj, port);
		break;

	case IIR_INT_ID_BUSY_DETECT:
	case IIR_INT_ID_NO_INTERRUPT_PENDING:
	default:
		break;
	}
}

/* Rockchip RK3288 UART */
