	.syntax unified
	.cpu cortex-a9
	.thumb
	.thumb_func
//
// dispatch.S
//

#include "common.h"
#include "arm.h"
#include "task.h"
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
**			| R0(DUMMY)			|+0
** SVC_SP->	| R4				|+4
**			| R5				|+8
**			| R6				|+12
**			| R7				|+16
**			| R8				|+20
**			| R9				|+24
**			| R10				|+28
**			| R11				|+32
** 			| LR(return address)|+36 <- _dispatch()の戻りアドレス または 割り込み復帰用スタブへのアドレス
**			----------------------
**  	  ** entry/IRQからの復帰 **
**			----------------------
**			| R0				|+40
**			| R1				|+44
**			| R2				|+48
**			| R3				|+52
**			| R12				|+56
**			| LR				|+60
**			| PC(return address)|+64  <- 割り込み発生時の戻りアドレス
**			| CPSR              |+68
**			---------------------
** beforeSP |					|+72
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

	/* save _ctask state (LR is _ctask) */
	push	{r0,r4-r11}
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
**	R0: _nodispatch_level
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
	.type   _dispatch_irq, %function
_dispatch_irq:
	/* check _nodispatch_level (R0 is _nodispatch_level) */
	cmp		r0, #0
	bne		_nodispatch_irq

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
	beq		_nosave_ctask /* _ctask is NULL ? */

	/* save current task context */
	adr		lr, _entry_stub
	push	{lr}
	push	{r0,r4-r11}

	/* save _ctask sp */
	str		sp, [r0,#SAVE_SP]

_nosave_ctask:
	b		_restore_context

_nodispatch_irq:
	b		_entry_stub

/*
**	_idle_loop
*/
_idle_loop:
	cps		#MODE_SVC
	ldr		sp, =stack_idle
1:
	bl		idle_task
	b		1b

/* stack for IDLE-TASK */
	.section .stack
	.balign 16
	.space	1024
stack_idle:
