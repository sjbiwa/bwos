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

/* タスクスタック上の位置(スタックに退避する情報に依存) */
#define	TASK_FRAME_SIZE		(4*22)	/* タスク情報退避エリアのサイズ(スタック上) */
#define	TASK_FRAME_FPEXC	(3)		/* タスク情報退避エリアのFPEXC位置 */
#define	TASK_FRAME_R0		(14)	/* タスク情報退避エリアのR0 */
#define	TASK_FRAME_R1		(15)	/* タスク情報退避エリアのR1 */
#define	TASK_FRAME_R2		(16)	/* タスク情報退避エリアのR2 */
#define	TASK_FRAME_R3		(17)	/* タスク情報退避エリアのR3 */
#define	TASK_FRAME_PC		(20)	/* タスク情報退避エリアのPC位置 */
#define	TASK_FRAME_PSR		(21)	/* タスク情報退避エリアのCPSR位置 */
#define	TASK_FRAME_STUB		(13)	/* タスク情報退避エリアの割り込みハンドラスタブ位置 */
#define	TASK_FRAME_SPSR		(0)		/* タスク情報退避エリアのSPSR_svc */
#define	TASK_FRAME_SP_USR	(1)		/* タスク情報退避エリアのSP_usr */
#define	TASK_FRAME_LR_USR	(2)		/* タスク情報退避エリアのLR_usr */

/* task_struct上の位置(task_structの構造に依存 */
#define	SAVE_SP				(8)		/* タスク構造体内にあるsave_spのオフセット */
#define	ARCH_TLS			(12)	/* ARCH固有のタスク情報(今はVFPレジスタ退避用アドレス)の位置 */
#define	SAVE_VFP			(12)	/* 上と同じ */
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
#define FPEXC_get()     ({uint32_t _reg_;__asm__ volatile ("vmrs %0,FPEXC":"=r"(_reg_)::"memory");_reg_;})
#define FPEXC_set(reg)  do {__asm__ volatile ("vmsr FPEXC,%0"::"r"(reg):"memory");} while (0)

/* コアID取得 */
#define	CPUID_get()		(MPIDR_get() & 0xff)

#else

/* コアID取得 */
#define	CPUID_get(reg)		MPIDR_get(reg);and reg, #0xff

#endif

#endif /* ARM_H_ */
