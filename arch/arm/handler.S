	.syntax unified
	.cpu cortex-a7
	.thumb
	.thumb_func

/*
 * handler.S
 *
 *  Created on: 2012/12/02
 *      Author: biwa
 */

#include "arm.h"
#include "cp15reg.h"

	.global _undef_entry
	.global _swi_entry
	.global _pref_entry
	.global _dabort_entry
	.global _irq_entry
	.global _fiq_entry
	.global cpsr_get
	.global cpsr_set
	.global	call_svc
	.global FPEXC_get
	.global FPEXC_set
	.type   _undef_entry, %function
	.type   _swi_entry, %function
	.type   _pref_entry, %function
	.type   _dabort_entry, %function
	.type   _irq_entry, %function
	.type   _fiq_entry, %function
	.type   cpsr_get, %function
	.type   cpsr_set, %function
	.type   call_svc, %function
	.type	FPEXC_get, %function
	.type	FPEXC_set, %function

	.text

_undef_entry:
	b		_undef_entry

_swi_entry:
	b		_swi_entry

_pref_entry:
	b		_pref_entry

_dabort_entry:
	DFSR_get(r0)
	DFAR_get(r1)
	b		_dabort_entry

/*
** _irq_entry
**
** タスク処理中の割り込み発生時の処理
** 8バイトアラインメント
**			stack_intr
**			----------------------
**SVC_SP	| old_SP(task SP)	|+0
**			| dummy(for align)	|+4
**			----------------------
** (bottom)
**
**			実行中タスクのスタック
**			----------------------
**old_SP	| FPEXC				|+0  (全タスク存在する)
**			| R0				|+4
**			| R1				|+8
**			| R2				|+12
**			| R3				|+16
**			| R12				|+20
**			| LR				|+24
**			| PC(return address)|+28 <- 割り込み発生時の戻りアドレス
**			| CPSR              |+32
**			---------------------
** beforeSP |					|+36
**
**
**
** 割り込み処理中の割り込み発生時の処理(多重割り込み)
** 8バイトアラインメント
**			stack_intr
**			----------------------
**SVC_SP	| old_SP			|+0(+0)
**			| DUMMY				|+4(+0) SPのアライン用(アライン時は存在しない)
**old_SP	| FPEXC				|+8(+4)
**			| R0				|+12(+8)
**			| R1				|+16(+12)
**			| R2				|+20(+16)
**			| R3				|+24(+20)
**			| R12				|+28(+24)
**			| LR				|+32(+28)
**			| PC(return address)|+36(+32)  <- 割り込み発生時の戻りアドレス
**			| CPSR              |+40(+36)
**			---------------------
** beforeSP |					|+44(+40)
**
*/
_irq_entry:
	sub		lr, lr, #4

	/* 状態をSVCスタックに退避してSVCモードに遷移 */
	srsdb	sp!, #MODE_SVC
	cps		#MODE_SVC
	push	{r0-r3,r12,lr}
	/* FPEXC get and VFP disable */
	vmrs	r1, FPEXC
	mov		r0, #0
	vmsr	FPEXC, r0
	push	{r1} /* FPEXC */

	/* SVC mode */

	/* INC _irq_level */
	ldr		r1, =_irq_level
	ldr		r0, [r1]
	add		r0, r0, #1
	str		r0, [r1]
	/* R0 is old _irq_level */

	/* check irq-level */
	cmp		r0, #1
	bne		_from_handler

_from_task:
	/* タスク実行中の割り込み */
	/* 割り込みハンドラで使うスタックを専用スタックに切り替え */
	mov		lr, sp
	ldr		r0, =stack_intr
	sub		r0, r0, #4		/* 8byte aligned */
	mov		sp, r0
	b		_common_ope

_from_handler:
	/* 割り込みハンドラ内での割り込み時はスタックはそのまま */
	/* Adjust SP 8bytes align */
	mov		lr, sp
	tst		lr, #0x07
	subne	sp, sp, #4

_common_ope:
	/* タスク/割り込み 共に以下からは共通処理 */
	push	{lr} /* old SP */

	ldr		r1, [lr,#28]
	ldr		r2, [lr,#32]
	mov		r0, lr

	bl		c_handler	/* r0:sp, r1:Return Address, r2:SPSR */

	/* restore SP */
	ldr		sp, [sp]

	/* restore FPEXC */
	pop		{r0}
	vmsr	FPEXC, r0

	/* DEC _irq_level */
	ldr		r1, =_irq_level
	ldr		r0, [r1]
	sub		r0, r0, #1
	str		r0, [r1]

	/* check _irq_level (R0 is _irq_level) */
	cmp		r0, #0
	bne		_nodispatch_irq

	b		_dispatch_irq

_fiq_entry:
	b		_fiq_entry


cpsr_get:
	mrs		r0, cpsr
	bx		lr


cpsr_set:
	msr		cpsr_fsxc, r0
	bx		lr

call_svc:
	push	{lr}
	svc		#0
	pop		{lr}
	bx		lr
