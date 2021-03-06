/*
 * Copyright (C) 2015 sjbiwa All rights reserved.
 */

/*
 * uart.h
 *
 *  Created on: 2015/03/30
 *      Author: biwa
 */

#ifndef UART_H_
#define UART_H_

#include "bwos.h"

#ifdef __cplusplus
extern "C" {
#endif

/* BaudRate */
#define	UART_BAUD460800					(460800)
#define	UART_BAUD230400					(230400)
#define	UART_BAUD115200					(115200)
#define	UART_BAUD57600					(57600)
#define	UART_BAUD38400					(38400)
#define	UART_BAUD19200					(19200)
#define	UART_BAUD9600					(9600)

/* bit_width */
#define	UART_BITS_5						(0)
#define	UART_BITS_6						(1)
#define	UART_BITS_7						(2)
#define	UART_BITS_8						(3)

/* Parity */
#define	UART_PARITY_NONE				(0)
#define	UART_PARITY_EVEN				(1)
#define	UART_PARITY_ODD					(2)

/* stop bit */
#define	UART_STOPBIT_1					(0)
#define	UART_STOPBIT_1_5				(1)
#define	UART_STOPBIT_2					(2)

/* flow control */
#define	UART_FLOW_NONE					(0)
#define	UART_FLOW_HARD					(1)
#define	UART_FLOW_SOFT					(2)	/* not supported (RTS/CTS soft control) */
#define	UART_FLOW_META					(3)	/* not supported (ex: XON/XOFF control) */

/* UART error bit */
#define	UART_ERR_FRAMING				(0x1u<<0)
#define	UART_ERR_PARITY					(0x1u<<1)
#define	UART_ERR_OVERRUN				(0x1u<<2)


typedef	struct {
	uint32_t		io_addr;				/* ベースアドレス */
	uint32_t		irq;					/* IRQ番号 */
	uint32_t		clock_src;				/* クロック識別子 */
} UartDeviceInfo;

typedef	struct {
	uint32_t		baudrate;				/* ボーレート */
	uint32_t		bits;					/* データビット幅 */
	uint32_t		parity;					/* パリティ設定 */
	uint32_t		stop_bits;				/* ストップビット幅 */
	uint32_t		flow_control;			/* フロー制御 */
} UartConfigParam;

typedef	struct {
	int				tx_buff_size;
	int				rx_buff_size;
} UartOpenParam;

extern void uart_register(UartDeviceInfo* info, uint32_t info_num);
extern void uart_set_config(uint32_t port_no, UartConfigParam* config);
extern void uart_open(uint32_t port_no, UartOpenParam* open);
extern void uart_close(uint32_t port_no);
extern int uart_send(uint32_t port_no, void* buff, int length, TimeOut tmout);
extern int uart_recv(uint32_t port_no, void* buff, int length, TimeOut tmout);
extern int uart_error(uint32_t port_no, uint32_t* err_bits, TimeOut tmout);

#ifdef __cplusplus
}
#endif

#endif /* UART_H_ */
