	.syntax unified
	.cpu cortex-a7
	.thumb
	.thumb_func
//
// dispatch.S
//

#include "arm.h"
#include "cp15reg.h"

	.text
	.balign 4

/**
** _dispatch
**    LR: return address
**    SVCモード/割り込み禁止で呼び出すこと
**
**       TASK_CONTEXT
**			---------------------
** SVC_SP->	| FPEXC				|+0
**			| R0				|+4
** 			| R4				|+8
**			| R5				|+12
**			| R6				|+16
**			| R7				|+20
**			| R8				|+24
**			| R9				|+28
**			| R10				|+32
**			| R11				|+36
** 			| LR(return address)|+40 <- _dispatch()の戻りアドレス または 割り込み復帰用スタブへのアドレス
**			----------------------
**  	  ** entry/IRQからの復帰 **
**			----------------------
**			| R0				|+44
**			| R1				|+48
**			| R2				|+52
**			| R3				|+56
**			| R12				|+60
**			| LR				|+64
**			| PC(return address)|+68  <- 割り込み発生時の戻りアドレス
**			| CPSR              |+72
**			---------------------
** beforeSP |					|+76
**
**		import _ctask	(current task)
**		import _ntask	(next task)
*/
	.global _dispatch
	.type   _dispatch, %function

_dispatch:

	push	{lr}

	/* check _ctask	*/
	ldr		lr, =_ctask
	ldr		lr, [lr]
	cmp		lr, #0

	/* IF _ctask not available ignore save_context */
	addeq	sp, sp, #4
	beq		_restore_context

_save_and_restore:
	/* save _ctask state (LR is _ctask) */
	push	{r0,r4-r11}
	/* save FPEXC */
	vmrs	r1, FPEXC
	push	{r1}
	/*********************************/
	/* if FPEXC.EN==1 save VFP !!!!! */
	/*********************************/
	tst		r1, #0x40000000
	beq		_no_save_vfp
	ldr		r1, [lr, #SAVE_VFP]
	vstmia	r1!, {d0-d15}
	vstmia	r1!, {d16-d31}
	vmrs	r0, FPSCR
	str		r0, [r1]

_no_save_vfp:
	str		sp, [lr,#SAVE_SP]

_restore_context:
	/* All register done saved */

	/* copy _ntask -> _ctask */
	ldr		r0, =_ctask
	ldr		lr, =_ntask
	ldr		lr, [lr]
	str		lr, [r0]

	/* check _ntask */
	cmp		lr, #0
	beq		_idle_loop

	/* restore SP (LR is _ntask)*/
	ldr		sp, [lr,#SAVE_SP]
	/* restore FPEXC */
	pop		{r1}
	vmsr	FPEXC, r1
	/*********************************/
	/* if FPEXC.EN==1 restore VFP !!!!! */
	/*********************************/
	tst		r1, #0x40000000
	beq		_no_load_vfp
	ldr		r1, [lr, #SAVE_VFP]
	vldmia	r1!, {d0-d15}
	vldmia	r1!, {d16-d31}
	ldr		r0, [r1]
	vmsr	FPSCR, r0

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
	/* compare _ctask and _ntask */
	ldr		r0, =_ctask
	ldr		r0, [r0]
	ldr		r1, =_ntask
	ldr		r1, [r1]
	cmp		r0, r1
	beq		_nodispatch_irq /* _ctask == _ntask ? */

	/* r0:_ctask r1:_ntask */
	/* _ctask check */
	cmp		r0, #0
	beq		_restore_context /* _ctask is NULL ? */

	/* save current task context */
	ldr		lr, =_entry_stub
	push	{lr}
	mov		lr, r0
	b		_save_and_restore

_nodispatch_irq:
	b		_entry_stub

/*
**	_idle_loop
*/
	.type   _idle_loop, %function
_idle_loop:
	cps		#MODE_SVC
	ldr		sp, =stack_idle
1:
	bl		idle_task
	b		1b
