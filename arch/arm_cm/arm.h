/*
 * arm.h
 *
 *  Created on: 2015/07/01
 *      Author: biwa
 */

#ifndef ARM_H_
#define ARM_H_

#define	FLAG_T			(0x1<<24)


/* スタックサイズ */
#define	IDLE_TASK_STACK_SIZE_SHIFT		8
#define	IRQ_STACK_SIZE_SHIFT			9
#define	IDLE_TASK_STACK_SIZE			(1<<IDLE_TASK_STACK_SIZE_SHIFT)
#define	IRQ_STACK_SIZE					(1<<IRQ_STACK_SIZE_SHIFT)


/* タスクスタック上の位置(スタックに退避する情報に依存) */
#define	TASK_FRAME_SIZE			(4*17)	/* タスク情報退避エリアのサイズ(スタック上) */
#define	TASK_FRAME_EXC_RETURN	(8)		/* タスク情報退避エリアのEXC_RETURN */
#define	TASK_FRAME_R0			(9)		/* タスク情報退避エリアのR0 */
#define	TASK_FRAME_R1			(10)	/* タスク情報退避エリアのR1 */
#define	TASK_FRAME_R2			(11)	/* タスク情報退避エリアのR2 */
#define	TASK_FRAME_R3			(12)	/* タスク情報退避エリアのR3 */
#define	TASK_FRAME_R12			(13)	/* タスク情報退避エリアのR12 */
#define	TASK_FRAME_LR			(14)	/* タスク情報退避エリアのRLR */
#define	TASK_FRAME_PC			(15)	/* タスク情報退避エリアのPC位置 */
#define	TASK_FRAME_PSR			(16)	/* タスク情報退避エリアのCPSR位置 */


/* ※下記定義は構造体メンバのオフセット値なのでコンパイル時にエラーチェックが可能なはず */
/* TaskStruct上の位置(TaskStructの構造に依存) */
#define	SAVE_SP				(8)		/* タスク構造体内にあるsave_spのオフセット */
#define	ARCH_TLS			(12)	/* ARCH固有のタスク情報の位置 */
/* CpuStruct上の位置(TaskStructの構造に依存) */
#define	CPUSTRUCT_CTASK		(0)		/* _ctaskのオフセット */
#define	CPUSTRUCT_NTASK		(4)		/* _ntaskのオフセット */

#ifndef __ASM__

#define	__dmb()				__asm__ volatile ("dmb":::"memory")
#define	__dsb()				__asm__ volatile ("dsb":::"memory")
#define	__isb()				__asm__ volatile ("isb":::"memory")
#define	__swi()				__asm__ volatile ("swi 0":::"memory")
#define	__wfi()				__asm__ volatile ("dsb;wfi":::"memory")
#define	__wfe()				__asm__ volatile ("dsb;wfe":::"memory")
#define	__sev()				__asm__ volatile ("dsb;sev":::"memory")

#define	sync_barrier()		__dsb()
#define	order_barrier()		__dmb()
#define	optimize_barrier()	__asm__ volatile ("":::"memory")

/* コアID取得 */
#define	CPUID_get()		(0)

#else /* __ASM__ */

/* コアID取得 */
#define	CPUID_get(reg)		mov reg, #0

#endif

#endif /* ARM_H_ */
