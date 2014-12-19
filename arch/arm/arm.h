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

#define	TASK_FRAME_SIZE		(4*18)	/* タスク情報退避エリアのサイズ(スタック上) */
#define	TASK_FRAME_PC		(16)	/* タスク情報退避エリアのPC位置 */
#define	TASK_FRAME_PSR		(17)	/* タスク情報退避エリアのCPSR位置 */
#define	TASK_FRAME_STUB		(9)		/* タスク情報退避エリアの割り込みハンドラスタブ位置 */
#define	SAVE_SP				(8)		/* タスク構造体内にあるsave_spのオフセット */

#ifndef __ASM__

#define	__dmb()				__asm__ volatile ("dmb":::"memory")
#define	__dsb()				__asm__ volatile ("dsb":::"memory")
#define	__isb()				__asm__ volatile ("isb":::"memory")
#define	__swi()				__asm__ volatile ("swi 0":::"memory")
#define	__wfi()				__asm__ volatile ("dsb;wfi":::"memory")
#define	__wfe()				__asm__ volatile ("dsb;wfe":::"memory")

/* VFPレジスタアクセス */
#define FPSCR_get()     ({uint32_t _reg_;__asm__ volatile ("vmrs %0,FPSCR":"=r"(_reg_)::"memory");_reg_;})
#define FPSCR_set(reg)  do {__asm__ volatile ("vmsr FPSCR,%0"::"r"(reg):"memory");} while (0)


#else
#endif

#endif /* ARM_H_ */
