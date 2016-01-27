/*
 * Copyright (C) 2015 sjbiwa All rights reserved.
 */

	.syntax unified
	.thumb
	.thumb_func
//
// dispatch.S
//

#include "arm.h"
#include "cp15reg.h"

	.text
	.balign 4

#if USE_SMP == 1
/*
** void _detach_active_task(CpuStruct* cpu, void* aff_info);
**     R0: CpuStruct* cpu
**     R1: void* aff_info
*/
	.global _detach_active_task
	.type   _detach_active_task, %function
	.type   _do_post_aff, %function
_detach_active_task:
	push	{lr}
	ldr		r2, [r0, #CPUSTRUCT_CTASK]
	b		_save_context

_do_post_aff:
	/* 現タスクのコンテキストは保存されたのでスタックを切り替える */
	/* _attach_active_taskが呼び出されるまでの間にIDLEスタックは  */
	/* 使われないので、これを使う                                 */
	CPUID_get(r12)
	ldr		r3, =stack_idle
	sub		r3, r3, r12, LSL #IDLE_TASK_STACK_SIZE_SHIFT
	mov		sp, r3

	mov		r0, r1 /* r0 <- aff_info */
	b		do_set_affinity

/*
** void _attach_active_task(CpuStruct* cpu);
**     R0: TaskStruct* task
*/
	.global _attach_active_task
	.type   _attach_active_task, %function

_attach_active_task:
	b		_skip_save_context

#endif

/**
** _switch_to
**    R0: CpuStruct* cpu_struct
**    LR: return address
**    SVCモード/割り込み禁止で呼び出すこと
**
**       TASK_CONTEXT
**			---------------------
** SVC_SP-> | SPSR_svc			|+0
** 			| SP_usr			|+4
**			| LR_usr			|+8
**			---------------------
** SVC_SP->	| FPEXC				|+12
**			| R0				|+16
** 			| R4				|+20
**			| R5				|+24
**			| R6				|+28
**			| R7				|+32
**			| R8				|+36
**			| R9				|+40
**			| R10				|+44
**			| R11				|+48
** 			| LR(return address)|+52 <- _dispatch()の戻りアドレス または 割り込み復帰用スタブへのアドレス
**			----------------------
**  	  ** entry/IRQからの復帰 **
**			----------------------
**			| R0				|+56
**			| R1				|+60
**			| R2				|+64
**			| R3				|+68
**			| R12				|+72
**			| LR				|+76
**			| PC(return address)|+80  <- 割り込み発生時の戻りアドレス
**			| CPSR              |+84
**			---------------------
** beforeSP |					|+88
**
*/
	.global _switch_to
	.type   _switch_to, %function

_switch_to:
	/* ※ 関数として呼ばれるエントリなので R0/R1/R2/R3/R12は破壊されても良い */
	push	{lr}

	mov		r1, #0 /* 通常のdispatch処理用 (r1!=0の時はset_affinity) */

	/* load and check _ctask */
	ldr		r2, [r0, #CPUSTRUCT_CTASK]
	cmp		r2, #0

	/* IF _ctask not available ignore save_context */
	beq		_skip_save_context

_save_context:
	/* R0:CpuStruct R1:dispatch/affinity指定 R2:ctask */
	/* R3/R12/LRはテンポラリとして使用可能 */

	/* save _ctask state */
	push	{r0,r4-r11}
	/* save FPEXC */
	vmrs	r3, FPEXC
	push	{r3}
	/*********************************/
	/* if FPEXC.EN==1 save VFP !!!!! */
	/*********************************/
	tst		r3, #0x40000000
	beq		_no_save_vfp
	ldr		r3, [r2, #SAVE_VFP]
	vstmia	r3!, {d0-d15}
	vstmia	r3!, {d16-d31}
	vmrs	r4, FPSCR
	str		r4, [r3]

_no_save_vfp:
	/* save USR/SYS info */
	mrs		r4, SPSR_svc
	mrs		r5, SP_usr
	mrs		r6, LR_usr
	push	{r4,r5,r6}

	str		sp, [r2,#SAVE_SP]

#if USE_SMP == 1
	/* dispatch/affinity 処理チェック */
	cmp		r1, #0
	bne		_do_post_aff
#endif

_skip_save_context:
	/* R0: cpu_struct */

	/* IDLEタスク/割り込み受付用スタックに切り替え */
	CPUID_get(r12)
	ldr		r3, =stack_idle
	sub		r3, r3, r12, LSL #IDLE_TASK_STACK_SIZE_SHIFT
	mov		sp, r3

	/* loadn and check _ntask */
	ldr		r2, [r0, #CPUSTRUCT_NTASK]
	/* store _ctask <- _ntask */
	str		r2, [r0, #CPUSTRUCT_CTASK]
	/* dispatchするタスクが無い場合はIDLEタスクへ */
	cmp		r2, #0
	beq		_idle_loop

	/* 割り込み許可して保留中の割り込みを受付     */
	/* ※ディスパッチしないようにディスパッチ禁止 */
	ldr		r3, =_dispatch_disable
	add		r3, r3, r12
	/* ディスパッチ禁止/割り込み許可 */
	mov		r12, #1
	strb	r12, [r3]
	cpsie	i
	/**********************/
	/* 外部割り込み受付中 */
	/**********************/
	/* 割り込み禁止/ディスパッチ許可 */
	cpsid	i
	mov		r12, #0
	strb	r12, [r3]

	/* restore SP (R2 is _ntask)*/
	ldr		sp, [r2,#SAVE_SP]

	/* restore USR/SYS info */
	pop		{r4,r5,r6,r7}
	msr		SPSR_svc, r4
	msr		SP_usr, r5
	msr		LR_usr, r6
	vmsr	FPEXC, r7
	/*********************************/
	/* if FPEXC.EN==1 restore VFP !!!!! */
	/*********************************/
	tst		r7, #0x40000000
	beq		_no_load_vfp
	ldr		r3, [r2, #SAVE_VFP]
	vldmia	r3!, {d0-d15}
	vldmia	r3!, {d16-d31}
	ldr		r4, [r3]
	vmsr	FPSCR, r4

_no_load_vfp:
	/* restore general-regs and return from _dispatch */
	pop		{r0,r4-r11,pc}

/*
**	_entry_stub
**		タスクエントリーから起動するためのスタブ
*/
	.global _entry_stub
	.type   _entry_stub, %function
_entry_stub:
	pop		{r0-r3,r12,lr}
	rfeia	sp!

/*
**  _dispatch_irq
**
**  MODE:SVC
**  IRQ:disable
**	FPEXC: current-TASK FP-MODE
**
**			----------------------
**SVC_SP	| R0				|+40
**			| R1				|+44
**			| R2				|+48
**			| R3				|+52
**			| R12				|+56
**			| LR				|+60
**			| PC(return address)|+64  <- 割り込み発生時の戻りアドレス
**			| CPSR              |+68
**			---------------------
** beforePS |					|+72
*/

	.global _dispatch_irq
	.global	_nodispatch_irq
	.type   _dispatch_irq, %function
	.type   _nodispatch_irq, %function

_dispatch_irq:
	/* ※ R0/R1/R2/R3/R12/LRはすでに退避されているので破壊されても良い */

	bl		get_cpu_struct
	/* R0 is cpu_struct */
	ldr		r1, [r0, #CPUSTRUCT_CTASK] 	/* r0 <- ctask */
	ldr		r2, [r0, #CPUSTRUCT_NTASK]	/* r1 <- ntask */
	/* compare _ctask and _ntask */
	cmp		r1, r2
	beq		_nodispatch_irq /* _ctask == _ntask ? */

	/* R0 is cpu_struct */
	ldr		lr, =_entry_stub
	b		_switch_to

_nodispatch_irq:
	b		_entry_stub

/*
**	_idle_loop
*/
	.type   _idle_loop, %function
_idle_loop:
	bl		idle_task
	b		_idle_loop
