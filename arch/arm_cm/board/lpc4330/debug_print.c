/*
 * Copyright (C) 2015 syujibiwa All rights reserved.
 */

/*
 * debug_print.c
 *
 *  Created on: 2015/07/01
 *      Author: biwa
 */
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include "kernel.h"

#define ITM_Port8(n)    (*((volatile unsigned char *)(0xE0000000+4*n)))
#define ITM_Port16(n)   (*((volatile unsigned short*)(0xE0000000+4*n)))
#define ITM_Port32(n)   (*((volatile unsigned long *)(0xE0000000+4*n)))

#define DEMCR           (*((volatile unsigned long *)(0xE000EDFC)))
#define TRCENA          0x01000000

static void debug_putc(int ch)
{
  if (DEMCR & TRCENA) {
    while (ITM_Port32(0) == 0);
    ITM_Port8(0) = ch;
  }
}

void debug_print(uint8_t* str)
{
	for (;*str;str++) {
		debug_putc(*str);
	}
}

void debug_print_init(void)
{
}
