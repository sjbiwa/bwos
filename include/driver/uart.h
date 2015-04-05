/*
 * uart.h
 *
 *  Created on: 2015/03/30
 *      Author: biwa
 */

#ifndef UART_H_
#define UART_H_

#include "bwos.h"

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


typedef	struct {
	uint8_t*		io_addr;				/* ベースアドレス */
	uint32_t		irq;					/* IRQ番号 */
	uint32_t		clock_src;				/* クロック識別子 */
} UartDeviceInfo;

typedef	struct {
	uint32_t		baudrate;				/* ボーレート */
	uint32_t		bits;					/* データビット幅 */
	uint32_t		parity;					/* パリティ設定 */
	uint32_t		stop_bits;				/* ストップビット幅 */
} UartConfigParam;

typedef	struct {
	int				tx_buff_size;
	int				rx_buff_size;
} UartOpenParam;

extern void uart_register(UartDeviceInfo* info, uint32_t info_num);
extern void uart_setConfig(uint32_t port_no, UartConfigParam* config);
extern void uart_open(uint32_t port_no, UartOpenParam* open);
extern void uart_close(uint32_t port_no);
extern int uart_send(uint32_t port_no, void* buff, int length, TimeOut tmout);
extern int uart_recv(uint32_t port_no, void* buff, int length, TimeOut tmout);

#endif /* UART_H_ */
