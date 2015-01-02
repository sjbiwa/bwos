/*
 * arch_common.h
 *
 *  Created on: 2014/12/07
 *      Author: biwa
 */

#ifndef ARCH_COMMON_H_
#define ARCH_COMMON_H_

#include "my_board.h"
#include "arm.h"

#define	irq_enable()		__asm__ volatile ("cpsie i":::"memory")
#define	irq_disable()		__asm__ volatile ("cpsid i":::"memory")

#define	irq_save(reg)		__asm__ volatile ("mrs %0, cpsr;cpsid i":"=r"(reg)::"memory")
#define	irq_restore(reg)	do { if ( !((reg) & FLAG_I) ) __asm__ volatile ("cpsie i":::"memory"); } while(0)

extern bool arch_can_dispatch(void);
extern TimeSpec get_tick_count(void);
extern void update_first_timeout(TimeSpec tmout);

#endif /* ARCH_COMMON_H_ */
