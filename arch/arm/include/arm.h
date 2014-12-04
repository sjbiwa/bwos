/*
 * arm.h
 *
 *  Created on: 2012/12/02
 *      Author: biwa
 */

#ifndef ARM_H_
#define ARM_H_

#define	MODE_USR		(0b10000)
#define	MODE_FIQ		(0b10001)
#define	MODE_IRQ		(0b10010)
#define	MODE_SVC		(0b10011)
#define	MODE_ABT		(0b10111)
#define	MODE_UND		(0b11011)
#define	MODE_SYS		(0b11111)

#define	FLAG_I			(0x80)
#define	FLAG_F			(0x40)
#define	FLAG_T			(0x20)

#ifndef __ASM__

#define	__dmb()				__asm__ volatile ("dmb":::"memory")
#define	__dsb()				__asm__ volatile ("dsb":::"memory")
#define	__isb()				__asm__ volatile ("isb":::"memory")
#define	__swi()				__asm__ volatile ("swi 0":::"memory")
#define	__wfi()				__asm__ volatile ("dsb;wfi":::"memory")
#define	__wfe()				__asm__ volatile ("dsb;wfe":::"memory")

#define	irq_enable()		__asm__ volatile ("cpsie i":::"memory")
#define	irq_disable()		__asm__ volatile ("cpsid i":::"memory")

#define	irq_save(reg)		__asm__ volatile ("mrs %0, cpsr;cpsid i":"=r"(reg)::"memory")
#define	irq_restore(reg)	do { if ( !((reg) & FLAG_I) ) __asm__ volatile ("cpsie i":::"memory"); } while(0)

#else
#endif

#endif /* ARM_H_ */
