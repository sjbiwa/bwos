/*
 * Copyright (C) 2015 sjbiwa All rights reserved.
 */

/*
 * arch_common.h
 *
 *  Created on: 2014/12/07
 *      Author: biwa
 */

#ifndef ARCH_COMMON_H_
#define ARCH_COMMON_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "my_board.h"
#include "arm.h"

#define	irq_enable()		__asm__ volatile ("cpsie i":::"memory")
#define	irq_disable()		__asm__ volatile ("cpsid i":::"memory")

#define	irq_save()			({ uint32_t _reg; __asm__ volatile ("mrs %0, cpsr;cpsid i":"=r"(_reg)::"memory"); _reg;})
#define	irq_restore(reg)	do { if ( !((reg) & FLAG_I) ) __asm__ volatile ("cpsie i":::"memory"); } while(0)

#define	STACK_ALIGN			(8)
#define	NORMAL_ALIGN		(8)

typedef	uint32_t			MemSize_t;	/* メモリサイズを表す型 */
typedef	uint32_t			PtrInt_t;	/* ポインタ型を整数型に変換するときの型 */

extern bool arch_can_dispatch(void);
extern TimeSpec get_tick_count(void);
extern void update_first_timeout(TimeSpec tmout);

#endif /* ARCH_COMMON_H_ */
