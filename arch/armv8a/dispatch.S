/*
 * Copyright (C) 2015 sjbiwa All rights reserved.
 */

#include "armv8.h"
#include "armv8reg.h"

#define	LED_ON(v)						ldr x8, =0x1C010008; mov w9, ##v; str w9, [x8]

	.text
	.balign 4

	.global _dispatch
	.type   _dispatch, %function

/*
**  _dispatch
*/
_dispatch:
/**
** _dispatch
**    X30(LR): return address
**    EL1/割り込み禁止で呼び出すこと
**
**       タスクコンテキスト
**          ------------
** SP->     | R19-29    |
**          ------------
**          | ret-stub  | <- 戻りアドレス IRQ(entry)/stubへ
**          ------------
**          | SP_EL0    | ※Systemタスクでは 0
**          ------------     ※API内部からの呼び出し時はRETで復帰
**          |  CPACR    |
**          -------------
**
**		import _ctask	(current task)
**		import _ntask	(next task)
*/
	.global _dispatch
	.type   _dispatch, %function

_dispatch:
	/* ※ 関数として呼ばれるエントリなので X0-X7は破壊されても良い */

	/* check _ctask	*/
	ldr		x1, =cpu_struct_pointer
	CPUID_get(x0)
	ldr		x1, [x1, x0, LSL #3]		/* x1 <- cpu_struct */
	ldr		x0, [x1, #CPUSTRUCT_CTASK] /* x0 <- ctask */
	cmp		x0, #0

	/* IF _ctask not available ignore save_context */
	beq		_restore_context

_save_and_restore:
	/* x0 is ctask */
	SP_EL0_get(x8)
	CPACR_EL1_get(x9)
	stp		 x8,  x9, [sp, #-16]!
	stp		x29, x30, [sp, #-16]!
	stp		x27, x28, [sp, #-16]!
	stp		x25, x26, [sp, #-16]!
	stp		x23, x24, [sp, #-16]!
	stp		x21, x22, [sp, #-16]!
	stp		x19, x20, [sp, #-16]!

	/* CPACR check */
	mov		w8, #0x00300000
	and		w9, w9, w8
	cmp		w8, w9
	bne		no_vfp_save
	/*******************/
	/* FPUレジスタ退避 */
	/*******************/
	ldr		x1, [x0, #SAVE_VFP]
	stp		 q0,  q1, [x1], #32
	stp		 q2,  q3, [x1], #32
	stp		 q4,  q5, [x1], #32
	stp		 q6,  q7, [x1], #32
	stp		 q8,  q9, [x1], #32
	stp		q10, q11, [x1], #32
	stp		q12, q13, [x1], #32
	stp		q14, q15, [x1], #32
	stp		q16, q17, [x1], #32
	stp		q18, q19, [x1], #32
	stp		q20, q21, [x1], #32
	stp		q22, q23, [x1], #32
	stp		q24, q25, [x1], #32
	stp		q26, q27, [x1], #32
	stp		q28, q29, [x1], #32
	stp		q30, q31, [x1], #32
	FPCR_get(x8)
	FPSR_get(x9)
	stp		x8, x9, [x1]

no_vfp_save:
	mov		x1, sp
	str		x1, [x0,#SAVE_SP]

_restore_context:
	/* All register done saved */

	/* copy _ntask -> _ctask */
	ldr		x19, =cpu_struct_pointer
	CPUID_get(x0)
	ldr		x19, [x19, x0, LSL #3]		/* get cpu_struct */
#if USE_SMP==1
	add		x0, x19, #CPUSTRUCT_LOCK
	bl		spin_lock
#endif
	ldr		x0, [x19, #CPUSTRUCT_NTASK]	/* x0 <- ntask */
	str		x0, [x19, #CPUSTRUCT_CTASK]	/* x0 -> ctask */
#if USE_SMP==1
	add		x0, x19, #CPUSTRUCT_LOCK
	bl		spin_unlock
#endif

	/* check _ntask */
	cmp		x0, #0
	beq		_idle_loop

	/* restore SP (X0 is _ntask)*/
	ldr		x1, [x0,#SAVE_SP]
	mov		sp, x1

	ldp		x19, x20, [sp], #16
	ldp		x21, x22, [sp], #16
	ldp		x23, x24, [sp], #16
	ldp		x25, x26, [sp], #16
	ldp		x27, x28, [sp], #16
	ldp		x29, x30, [sp], #16
	ldp		 x8,  x9, [sp], #16
	SP_EL0_set(x8)
	CPACR_EL1_set(x9)

	/* CPACR check */
	mov		w8, #0x00300000
	and		w9, w9, w8
	cmp		w8, w9
	bne		no_vfp_restore
	/*******************/
	/* FPUレジスタ復帰 */
	/*******************/
	ldr		x1, [x0, #SAVE_VFP]
	ldp		 q0,  q1, [x1], #32
	ldp		 q2,  q3, [x1], #32
	ldp		 q4,  q5, [x1], #32
	ldp		 q6,  q7, [x1], #32
	ldp		 q8,  q9, [x1], #32
	ldp		q10, q11, [x1], #32
	ldp		q12, q13, [x1], #32
	ldp		q14, q15, [x1], #32
	ldp		q16, q17, [x1], #32
	ldp		q18, q19, [x1], #32
	ldp		q20, q21, [x1], #32
	ldp		q22, q23, [x1], #32
	ldp		q24, q25, [x1], #32
	ldp		q26, q27, [x1], #32
	ldp		q28, q29, [x1], #32
	ldp		q30, q31, [x1], #32
	ldp		x8, x9, [x1]
	FPCR_set(x8)
	FPSR_set(x9)

no_vfp_restore:

	ret

/*
**	_entry_stub
**		タスクエントリーから起動するためのスタブ
**
**         ------------
** SP->    | ELR_EL1   |
**         ------------
**         | SPSR_EL1  |
**         ------------
**         | R0-18/30  |
**         ------------
**
*/
	.global _entry_stub
	.type   _entry_stub, %function
_entry_stub:
	ldp		 x8,  x9, [sp], #16
	ELR_EL1_set(x8)
	SPSR_EL1_set(x9)

	ldp		 x0,  x1, [sp], #16
	ldp		 x2,  x3, [sp], #16
	ldp		 x4,  x5, [sp], #16
	ldp		 x6,  x7, [sp], #16
	ldp		 x8,  x9, [sp], #16
	ldp		x10, x11, [sp], #16
	ldp		x12, x13, [sp], #16
	ldp		x14, x15, [sp], #16
	ldp		x16, x17, [sp], #16
	ldp		x18, x30, [sp], #16

	eret

/*
**  _dispatch_irq
*/

	.global _dispatch_irq
	.global	_nodispatch_irq
	.type   _dispatch_irq, %function
	.type   _nodispatch_irq, %function

_dispatch_irq:
	/* ※ X0-X18はすでに退避されているので破壊されても良い */
	/* compare _ctask and _ntask */
	ldr		x1, =cpu_struct_pointer
	CPUID_get(x0)
	ldr		x1, [x1, x0, LSL #3]		/* x1 <- cpu_struct */
	ldr		x0, [x1, #CPUSTRUCT_CTASK] 	/* x0 <- ctask */
	ldr		x1, [x1, #CPUSTRUCT_NTASK]	/* x1 <- ntask */
	cmp		x0, x1
	beq		_nodispatch_irq /* _ctask == _ntask ? */

	/* x0:_ctask x1:_ntask */
	/* _ctask check */
	cmp		x0, #0
	beq		_restore_context /* _ctask is NULL ? */

	/* save current task context */
	ldr		x30, =_entry_stub
	b		_save_and_restore


_nodispatch_irq:
	b		_entry_stub

/*
**	_idle_loop
*/
	.type   _idle_loop, %function
_idle_loop:
	ldr		x0, =stack_idle
	CPUID_get(x1)
	sub		x0, x0, x1, LSR #IDLE_TASK_STACK_SIZE_SHIFT
	mov		sp, x0
1:
	bl		idle_task
	b		1b
