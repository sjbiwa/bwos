	.syntax unified
	.cpu cortex-a9
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
**			----------------------
**	R0		| PC(return address)|+0
**			| CPSR				|+4
**			| R0				|+8
**			| R1				|+12
**			| R2				|+16
**			---------------------
** IRQ_SP	|					|+20
**
**					↓コピーする
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
** beforeSP |					|+72
*/
_irq_entry:
	sub		lr, lr, #4

	/* save PC/CPSR to SVC-stack */
#if defined(__QEMU__)
	/* save R0 to irq-stack */
	stmdb	sp, {r0,r1,r2}
	sub		r0, sp, #20

	/* save PC/CPSR to temporary */
	mov		r1, lr
	mrs		r2, spsr
	stmia	r0, {r1,r2}

	cps		#MODE_SVC

	/* R0=TEMP_STACK_TOP  R1,R2 is scratch */
	/* save PC/CPSR to SVC-stack */
	ldmia	r0!, {r1,r2}
	push	{r1,r2}

	/* restore R0-R2 */
	ldmia	r0, {r0,r1,r2}

	/* save R0-R3,R12,LR */
	push	{r0-r3,r12,lr}
#else
	srsdb	sp!, #MODE_SVC
	cps		#MODE_SVC
	push	{r0-r3,r12,lr}
#endif
	/* SVC mode */

	/* INC _nodispatch_level */
	ldr		r1, =_nodispatch_level
	ldr		r0, [r1]
	add		r0, r0, #1
	str		r0, [r1]

	/* FPEXC get and VFP disable */
	mrs		r1, FPEXC
	mov		r0, #0
	msr		FPEXC, r0

	/* Adjust SP 8bytes align */
	mov		lr, sp
	tst		lr, #0x07
	subne	sp, sp, #4
	push	{lr} /* old SP */
	push	{r1} /* FPEXC */

	ldr		r1, [lr,#24]
	ldr		r2, [lr,#28]
	mov		r0, lr

	bl		c_handler	/* r0:sp, r1:Return Address, r2:SPSR */

	/* restore FPEXC */
	pop		{r0}
	msr		FPEXC, r0

	/* restore SP */
	ldr		sp, [sp]

	/* DEC _nodispatch_level */
	ldr		r1, =_nodispatch_level
	ldr		r0, [r1]
	sub		r0, r0, #1
	str		r0, [r1]

	/* check _nodispatch_level (R0 is _nodispatch_level) */
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
