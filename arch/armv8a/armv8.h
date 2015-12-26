/*
 * Copyright (C) 2015 sjbiwa All rights reserved.
 */

/*
 * armv8.h
 *
 *  Created on: 2015/12/13
 *      Author: biwa
 */

#ifndef ARMV8_H_
#define ARMV8_H_

/* DAIF register */
#define	FLAG_D			(0x1u<<9)
#define	FLAG_A			(0x1u<<8)
#define	FLAG_I			(0x1u<<7)
#define	FLAG_F			(0x1u<<6)

/* スタックサイズ */
#define	EXCEPTION_STACK_SIZE_SHIFT		10
#define	IDLE_TASK_STACK_SIZE_SHIFT		10
#define	IRQ_STACK_SIZE_SHIFT			16
#define	EXCEPTION_STACK_SIZE			(1<<EXCEPTION_STACK_SIZE_SHIFT)
#define	IDLE_TASK_STACK_SIZE			(1<<IDLE_TASK_STACK_SIZE_SHIFT)
#define	IRQ_STACK_SIZE					(1<<IRQ_STACK_SIZE_SHIFT)


/* タスクスタック上の位置(スタックに退避する情報に依存) */
#define	TASK_FRAME_SIZE		(8*38)		/* タスク情報退避エリアのサイズ(スタック上) */
#define	TASK_FRAME_CPACR	(14)		/* タスク情報退避エリアのCPACR位置 */
#define	TASK_FRAME_X0		(18)		/* タスク情報退避エリアのX0 */
#define	TASK_FRAME_X1		(19)		/* タスク情報退避エリアのX1 */
#define	TASK_FRAME_X2		(20)		/* タスク情報退避エリアのX2 */
#define	TASK_FRAME_X3		(21)		/* タスク情報退避エリアのX3 */
#define	TASK_FRAME_PC		(16)		/* タスク情報退避エリアのPC位置 */
#define	TASK_FRAME_PSR		(17)		/* タスク情報退避エリアのSPSR位置 */
#define	TASK_FRAME_STUB		(2)			/* タスク情報退避エリアの割り込みハンドラスタブ位置 */
#define	TASK_FRAME_SP_EL0	(12)		/* タスク情報退避エリアのSP_EL0 */
#define	TASK_FRAME_LR		(37)		/* タスク情報退避エリアのLR */

/* TaskStruct上の位置(TaskStructの構造に依存) */
#define	SAVE_SP				()		/* タスク構造体内にあるsave_spのオフセット */
#define	ARCH_TLS			()	/* ARCH固有のタスク情報(今はVFPレジスタ退避用アドレス)の位置 */
#define	SAVE_VFP			()	/* 上と同じ */
/* CpuStruct上の位置(TaskStructの構造に依存) */
#define	CPUSTRUCT_CTASK		()		/* _ctaskのオフセット */
#define	CPUSTRUCT_NTASK		()		/* _ntaskのオフセット */
#define	CPUSTRUCT_LOCK		()		/* spinlockのオフセット */

#ifndef __ASM__

#define	__dmb()				__asm__ volatile ("dmb #0xf":::"memory")
#define	__dsb()				__asm__ volatile ("dsb #0xf":::"memory")
#define	__isb()				__asm__ volatile ("isb #0xf":::"memory")
#define	__swi()				__asm__ volatile ("swi 0":::"memory")
#define	__wfi()				__asm__ volatile ("dsb  #0xf;wfi":::"memory")
#define	__wfe()				__asm__ volatile ("dsb  #0xf;wfe":::"memory")
#define	__sev()				__asm__ volatile ("dsb  #0xf;sev":::"memory")

#define	sync_barrier()		__dsb()
#define	order_barrier()		__dmb()
#define	optimize_barrier()	__asm__ volatile ("":::"memory")

/* VFPレジスタアクセス */
#define FPSCR_get()     ({uint32_t _reg_;__asm__ volatile ("vmrs %0,FPSCR":"=r"(_reg_)::"memory");_reg_;})
#define FPSCR_set(reg)  do {__asm__ volatile ("vmsr FPSCR,%0"::"r"(reg):"memory");} while (0)
#define FPEXC_get()     ({uint32_t _reg_;__asm__ volatile ("vmrs %0,FPEXC":"=r"(_reg_)::"memory");_reg_;})
#define FPEXC_set(reg)  do {__asm__ volatile ("vmsr FPEXC,%0"::"r"(reg):"memory");} while (0)

/* コアID取得 */
#if USE_SMP==1
#define	CPUID_get()		(MPIDR_EL1_get() & 0xff)
#else
#define	CPUID_get()		(0)
#endif

#else /* __ASM__ */

/* コアID取得 */
#if USE_SMP==1
#define	CPUID_get(reg)		MPIDR_EL1_get(reg);and reg, #0xff
#else
#define	CPUID_get(reg)		mov reg, #0
#endif

#endif

#endif /* ARMV8_H_ */
