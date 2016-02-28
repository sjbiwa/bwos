/*
 * Copyright (C) 2015 sjbiwa All rights reserved.
 */

#include "armv8.h"
#include "armv8reg.h"

	.text
	.balign 4

#if USE_SMP == 1
/*
** void _detach_active_task(CpuStruct* cpu, void* aff_info);
**     X0: CpuStruct* cpu
**     X1: void* aff_info
*/
	.global _detach_active_task
	.type   _detach_active_task, %function
	.type   _do_post_aff, %function
_detach_active_task:
	ldr		x2, [x0, #CPUSTRUCT_CTASK]
	b		_save_context

_do_post_aff:
	/* 現タスクのコンテキストは保存されたのでスタックを切り替える */
	/* _attach_active_taskが呼び出されるまでの間にIDLEスタックは  */
	/* 使われないので、これを使う                                 */
	CPUID_get(x3)
	ldr		x4, =stack_idle
	sub		x4, x4, x3, LSL #IDLE_TASK_STACK_SIZE_SHIFT
	mov		sp, x4

	mov		x0, x1 /* x0 <- aff_info */
	b		do_set_affinity

/*
** void _attach_active_task(CpuStruct* cpu);
**     X0: TaskStruct* task
*/
	.global _attach_active_task
	.type   _attach_active_task, %function

_attach_active_task:
	b		_skip_save_context

#endif

	.global _switch_to
	.type   _switch_to, %function
/**
** _switch_to
**    X0: CpuStruct* cpu_struct
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
*/
_switch_to:
	/* ※ 関数として呼ばれるエントリなので X0-X7は破壊されても良い */

	mov		x1, #0 /* 通常のdispatch処理用 */

	/* load and check _ctask */
	ldr		x2, [x0, #CPUSTRUCT_CTASK]
	cbz		x2, _skip_save_context

_save_context:
	/* X0:CpuStruct X1:dispatch/affinity指定 X2:ctask */

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
	ldr		x4, [x2, #SAVE_VFP]
	stp		 q0,  q1, [x4], #32
	stp		 q2,  q3, [x4], #32
	stp		 q4,  q5, [x4], #32
	stp		 q6,  q7, [x4], #32
	stp		 q8,  q9, [x4], #32
	stp		q10, q11, [x4], #32
	stp		q12, q13, [x4], #32
	stp		q14, q15, [x4], #32
	stp		q16, q17, [x4], #32
	stp		q18, q19, [x4], #32
	stp		q20, q21, [x4], #32
	stp		q22, q23, [x4], #32
	stp		q24, q25, [x4], #32
	stp		q26, q27, [x4], #32
	stp		q28, q29, [x4], #32
	stp		q30, q31, [x4], #32
	FPCR_get(x8)
	FPSR_get(x9)
	stp		x8, x9, [x4]

no_vfp_save:
	mov		x4, sp
	str		x4, [x2,#SAVE_SP]

#if USE_SMP == 1
	/* dispatch/affinity 処理チェック */
	cbnz	x1, _do_post_aff
#endif

_skip_save_context:
	/* X0:CpuStruct */

	/* IDLEタスク/割り込み受付用スタックに切り替え */
	CPUID_get(x3)
	ldr		x4, =stack_idle
	sub		x4, x4, x3, LSL #IDLE_TASK_STACK_SIZE_SHIFT
	mov		sp, x4

#if 0
	/* 割り込み許可して保留中の割り込みを受付     */
	/* ※ディスパッチしないようにディスパッチ禁止 */
	ldr		x4, =_dispatch_disable
	add		x4, x4, x3
	/* ディスパッチ禁止/割り込み許可 */
	mov		w5, #1
	strb	w5, [x4]
	DAIFClr_seti(0x2)
	/**********************/
	/* 外部割り込み受付中 */
	/**********************/
	/* 割り込み禁止/ディスパッチ許可 */
	DAIFSet_seti(0x2)
	strb	wzr, [x4]
#endif

	/* loadn and check _ntask */
	ldr		x2, [x0, #CPUSTRUCT_NTASK]
	/* store _ctask <- _ntask */
	str		x2, [x0, #CPUSTRUCT_CTASK]
	/* dispatchするタスクが無い場合はIDLEタスクへ */
	cbz		x2, _idle_loop

	/* restore SP (X2 is _ntask)*/
	ldr		x4, [x2, #SAVE_SP]
	mov		sp, x4

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
	ldr		x4, [x2, #SAVE_VFP]
	ldp		 q0,  q1, [x4], #32
	ldp		 q2,  q3, [x4], #32
	ldp		 q4,  q5, [x4], #32
	ldp		 q6,  q7, [x4], #32
	ldp		 q8,  q9, [x4], #32
	ldp		q10, q11, [x4], #32
	ldp		q12, q13, [x4], #32
	ldp		q14, q15, [x4], #32
	ldp		q16, q17, [x4], #32
	ldp		q18, q19, [x4], #32
	ldp		q20, q21, [x4], #32
	ldp		q22, q23, [x4], #32
	ldp		q24, q25, [x4], #32
	ldp		q26, q27, [x4], #32
	ldp		q28, q29, [x4], #32
	ldp		q30, q31, [x4], #32
	ldp		x8, x9, [x4]
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

/*
	ELR_EL1_get(x0)
	SPSR_EL1_get(x1)
	SPSel_get(x2)
	CurrentEL_get(x3)
	mov		x4, sp
	bl		test_print
*/
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

	bl		get_cpu_struct
	/* x0 is cpu_struct */
	ldr		x1, [x0, #CPUSTRUCT_CTASK] 	/* x0 <- ctask */
	ldr		x2, [x0, #CPUSTRUCT_NTASK]	/* x1 <- ntask */
	/* compare _ctask and _ntask */
	cmp		x1, x2
	beq		_nodispatch_irq /* _ctask == _ntask ? */

	/* x0:_ctask */
	ldr		x30, =_entry_stub
	b		_switch_to

_nodispatch_irq:
	b		_entry_stub

/*
**	_idle_loop
*/
	.type   _idle_loop, %function
_idle_loop:
1:
	bl		idle_task
	b		1b
