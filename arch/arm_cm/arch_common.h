/*
 * Copyright (C) 2015 sjbiwa All rights reserved.
 */

/*
 * arch_common.h
 *
 *  Created on: 2015/07/01
 *      Author: biwa
 */

#ifndef ARCH_COMMON_H_
#define ARCH_COMMON_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "my_board.h"
#include "arm.h"

#define	irq_enable()		do { __asm__ volatile ("cpsie i":::"memory"); } while (0)
#define	irq_disable()		do { __asm__ volatile ("cpsid i":::"memory"); } while (0)

#define	irq_save()			({ uint32_t ret; __asm__ volatile ("MRS %0, PRIMASK; cpsid i":"=r"(ret)::"memory"); ret;})
#define	irq_restore(reg)	do { __asm__ volatile ("MSR PRIMASK, %0"::"r"(reg):"memory"); } while (0)

#define	xpsr_get()			({ uint32_t ret; __asm__ volatile ("MRS %0, xPSR":"=r"(ret)::"memory"); ret;})

#define	fpscr_set(reg)		do { __asm__ volatile ("VMSR FPSCR, %0"::"r"(reg):"memory"); } while (0)
#define	fpscr_get()			({ uint32_t ret; __asm__ volatile ("VMRS %0, FPSCR:"=r"(ret)"::"memory"); ret;})

#define	STACK_ALIGN			(8)
#define	NORMAL_ALIGN		(8)

typedef	uint32_t			MemSize_t;	/* メモリサイズを表す型 */
typedef	uint32_t			PtrInt_t;	/* ポインタ型を整数型に変換するときの型 */

#endif /* ARCH_COMMON_H_ */
