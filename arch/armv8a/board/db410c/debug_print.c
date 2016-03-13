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

#if 0
#else
static inline void debug_putc(uint8_t c)
{
}
#endif

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
#if 0
	iowrite32(UART_BASE+UARTIBRD, 0x6);
	iowrite32(UART_BASE+UARTFBRD, 0x21);
	iowrite32(UART_BASE+UARTLCR_H, 0x70);
	iowrite32(UART_BASE+UARTCR, 0x301);
#endif
}

void led_on(uint32_t led)
{
}

