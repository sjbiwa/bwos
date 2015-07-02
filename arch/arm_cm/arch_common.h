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

#define	irq_enable()		do { uint32_t _reg = 0; __asm__ volatile ("MSR PRIMASK, %0"::"r"(_reg):"memory"); } while (0)
#define	irq_disable()		do { uint32_t _reg = 1; __asm__ volatile ("MSR PRIMASK, %0"::"r"(_reg):"memory"); } while (0)

#define	irq_save()			({ uint32_t ret; uint32_t _reg = 1; __asm__ volatile ("MRS %0, PRIMASK;MSR PRIMASK, %1":"=r"(ret):"r"(_reg):"memory"); ret;})
#define	irq_restore(reg)	do { __asm__ volatile ("MSR PRIMASK, %0"::"r"(reg):"memory"); } while (0)

#define	STACK_ALIGN			(8)
#define	NORMAL_ALIGN		(8)

typedef	uint32_t			MemSize_t;	/* メモリサイズを表す型 */
typedef	uint32_t			PtrInt_t;	/* ポインタ型を整数型に変換するときの型 */

extern bool arch_can_dispatch(void);
extern TimeSpec get_tick_count(void);
extern void update_first_timeout(TimeSpec tmout);

#endif /* ARCH_COMMON_H_ */
