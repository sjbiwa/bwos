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

#define	MODE_SYS		(0x05u)
#define	MODE_USR		(0x00u)

/* スタックサイズ */
#define	EXCEPTION_STACK_SIZE_SHIFT		10
#define	IDLE_TASK_STACK_SIZE_SHIFT		10
#define	IRQ_STACK_SIZE_SHIFT			16
#define	EXCEPTION_STACK_SIZE			(1<<EXCEPTION_STACK_SIZE_SHIFT)
#define	IDLE_TASK_STACK_SIZE			(1<<IDLE_TASK_STACK_SIZE_SHIFT)
#define	IRQ_STACK_SIZE					(1<<IRQ_STACK_SIZE_SHIFT)


/* タスクスタック上の位置(スタックに退避する情報に依存) */
#define	TASK_FRAME_SIZE		(8*36)		/* タスク情報退避エリアのサイズ(スタック上) */
#define	TASK_FRAME_CPACR	(13)		/* タスク情報退避エリアのCPACR位置 */
#define	TASK_FRAME_X0		(16)		/* タスク情報退避エリアのX0 */
#define	TASK_FRAME_X1		(17)		/* タスク情報退避エリアのX1 */
#define	TASK_FRAME_X2		(18)		/* タスク情報退避エリアのX2 */
#define	TASK_FRAME_X3		(19)		/* タスク情報退避エリアのX3 */
#define	TASK_FRAME_X4		(20)		/* タスク情報退避エリアのX4 */
#define	TASK_FRAME_X5		(21)		/* タスク情報退避エリアのX5 */
#define	TASK_FRAME_X6		(22)		/* タスク情報退避エリアのX6 */
#define	TASK_FRAME_X7		(23)		/* タスク情報退避エリアのX7 */
#define	TASK_FRAME_X8		(24)		/* タスク情報退避エリアのX8 */
#define	TASK_FRAME_X9		(25)		/* タスク情報退避エリアのX9 */
#define	TASK_FRAME_X10		(26)		/* タスク情報退避エリアのX10 */
#define	TASK_FRAME_X11		(27)		/* タスク情報退避エリアのX11 */
#define	TASK_FRAME_X12		(28)		/* タスク情報退避エリアのX12 */
#define	TASK_FRAME_X13		(29)		/* タスク情報退避エリアのX13 */
#define	TASK_FRAME_X14		(30)		/* タスク情報退避エリアのX14 */
#define	TASK_FRAME_X15		(31)		/* タスク情報退避エリアのX15 */
#define	TASK_FRAME_X16		(32)		/* タスク情報退避エリアのX16 */
#define	TASK_FRAME_X17		(33)		/* タスク情報退避エリアのX17 */
#define	TASK_FRAME_X18		(34)		/* タスク情報退避エリアのX18 */
#define	TASK_FRAME_X19		(0)			/* タスク情報退避エリアのX19 */
#define	TASK_FRAME_X20		(1)			/* タスク情報退避エリアのX20 */
#define	TASK_FRAME_X21		(2)			/* タスク情報退避エリアのX21 */
#define	TASK_FRAME_X22		(3)			/* タスク情報退避エリアのX22 */
#define	TASK_FRAME_X23		(4)			/* タスク情報退避エリアのX23 */
#define	TASK_FRAME_X24		(5)			/* タスク情報退避エリアのX24 */
#define	TASK_FRAME_X25		(6)			/* タスク情報退避エリアのX25 */
#define	TASK_FRAME_X26		(7)			/* タスク情報退避エリアのX26 */
#define	TASK_FRAME_X27		(8)			/* タスク情報退避エリアのX27 */
#define	TASK_FRAME_X28		(9)			/* タスク情報退避エリアのX28 */
#define	TASK_FRAME_X29		(10)		/* タスク情報退避エリアのX29 */
#define	TASK_FRAME_PC		(14)		/* タスク情報退避エリアのPC位置 */
#define	TASK_FRAME_PSR		(15)		/* タスク情報退避エリアのSPSR位置 */
#define	TASK_FRAME_STUB		(11)		/* タスク情報退避エリアの割り込みハンドラスタブ位置 */
#define	TASK_FRAME_SP_EL0	(12)		/* タスク情報退避エリアのSP_EL0 */
#define	TASK_FRAME_LR		(35)		/* タスク情報退避エリアのLR */

/* TaskStruct上の位置(TaskStructの構造に依存) */
#define	SAVE_SP				(16)		/* タスク構造体内にあるsave_spのオフセット */
#define	ARCH_TLS			(24)		/* ARCH固有のタスク情報(今はVFPレジスタ退避用アドレス)の位置 */
#define	SAVE_VFP			(24)		/* 上と同じ */
/* CpuStruct上の位置(TaskStructの構造に依存) */
#define	CPUSTRUCT_CTASK		(0)			/* _ctaskのオフセット */
#define	CPUSTRUCT_NTASK		(8)			/* _ntaskのオフセット */
#define	CPUSTRUCT_LOCK		(16)		/* spinlockのオフセット */

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


/* コアID取得 */
#if USE_SMP==1
#define	CPUID_get()			(MPIDR_EL1_get() & 0xff)
#else
#define	CPUID_get()			(0)
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
