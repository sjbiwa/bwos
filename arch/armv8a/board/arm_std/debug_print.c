/*
 * Copyright (C) 2015 sjbiwa All rights reserved.
 */

/*
 * debug_print.c
 *
 *  Created on: 2015/02/13
 *      Author: biwa
 */
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include "kernel.h"
#include "bwos.h"

#define	UART_BASE			0x001C090000

#define	UARTDR				(0x000)				/* Data Register */
#define	UARTRSR				(0x004)				/* Receive Status Register */
#define	UARTECR				(0x004)				/* Error Clear Register */
#define	UARTFR				(0x018)				/* Flag Register */
#define	UARTILPR			(0x020)				/* IrDA Low-Power Counter Register */
#define	UARTIBRD			(0x024)				/* Integer Baud Rate Register */
#define	UARTFBRD			(0x028)				/* Fractional Baud Rate Register */
#define	UARTLCR_H			(0x02C)				/* Line Control Register */
#define	UARTCR				(0x030)				/* Control Register */
#define	UARTIFLS			(0x034)				/* Interrupt FIFO Level Select Register */
#define	UARTIMSC			(0x038)				/* Interrupt Mask Set/Clear Register */
#define	UARTRIS				(0x03C)				/* Raw Interrupt Status Register */
#define	UARTMIS				(0x040)				/* Masked Interrupt Status Register */
#define	UARTICR				(0x044)				/* Interrupt Clear Register */
#define	UARTDMACR			(0x048)				/* DMA Control Register */
#define	UARTPeriphID0		(0xFE0)				/* UARTPeriphID0 Register */
#define	UARTPeriphID1		(0xFE4)				/* UARTPeriphID1 Register */
#define	UARTPeriphID2		(0xFE8)				/* UARTPeriphID2 Register */
#define	UARTPeriphID3		(0xFEC)				/* UARTPeriphID3 Register */
#define	UARTPCellID0		(0xFF0)				/* UARTPCellID0 Register */
#define	UARTPCellID1		(0xFF4)				/* UARTPCellID1 Register */
#define	UARTPCellID2		(0xFF8)				/* UARTPCellID2 Register */
#define	UARTPCellID3		(0xFFC)				/* UARTPCellID3 Register */

static inline void debug_putc(uint8_t c)
{
	while (ioread32(UART_BASE+UARTFR) & (0x1<<5));
	iowrite32(UART_BASE+UARTDR, c);
}

void debug_print(uint8_t* str)
{
	for (;*str;str++) {
		debug_putc(*str);
		if ( *str == '\n' ) {
			debug_putc('\r');
		}
	}
}


void debug_print_init(void)
{
	iowrite32(UART_BASE+UARTIBRD, 0x6);
	iowrite32(UART_BASE+UARTFBRD, 0x21);
	iowrite32(UART_BASE+UARTLCR_H, 0x70);
	iowrite32(UART_BASE+UARTCR, 0x301);
}

void led_on(uint32_t led)
{
	iowrite32(SYSTEM_BASE+0x0008,  led);
}

