/*
 * uart.c
 *
 *  Created on: 2015/02/01
 *      Author: biwa
 */

#include "kernel.h"
#include "clock.h"

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
#define	IER_RECEIVE_DATA_AVAILABLE_INT_EN	(0x1u<<0)		/* Received Data Available */

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



/* Rockchip RK3288 UART */
