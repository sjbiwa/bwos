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

#define	UART_BASE		(0xFF690000u)

void debug_print(uint8_t* str)
{
	for (;*str;str++) {
		while ( (*(volatile uint32_t*)(UART_BASE+0x7C) & (0x01<<2)) == 0 );
		*((volatile uint32_t*)(UART_BASE+0x00)) = *str;
	}
}

void debug_print_init(void)
{
}
