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

#define	RESET_STACK_NO				(0)
#define	RESET_ENTRY_NO				(1)
#define	NMI_ENTRY_NO				(2)
#define	HARD_FAULT_ENTRY_NO			(3)
#define	MEM_MANAGE_ENTRY_NO			(4)
#define	BUS_FAULT_ENTRY_NO			(5)
#define	USAGE_FAULT_ENTRY_NO		(6)
#define	SVC_ENTRY_NO				(11)
#define	DEBUG_ENTRY_NO				(12)
#define	PENDSVC_ENTRY_NO			(14)
#define	SYSTICK_ENTRY_NO			(15)


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

extern bool arch_can_dispatch(void);
extern TimeSpec get_tick_count(void);
extern void update_first_timeout(TimeSpec tmout);

#endif /* ARCH_COMMON_H_ */
