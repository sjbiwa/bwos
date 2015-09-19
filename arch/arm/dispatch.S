/*
 * Copyright (C) 2015 syujibiwa All rights reserved.
 */

	.syntax unified
	.cpu cortex-a7
	.thumb
	.thumb_func
//
// dispatch.S
//

#include "arm.h"
#include "cp15reg.h"

/* cpu_struct/ctaskを取得 */
#define	GET_CTASK(cpu_struct,ctask) \
	ldr		cpu_struct, =cpu_struct_pointer; \
	CPUID_get(ctask); \
	ldr		cpu_struct, [cpu_struct, ctask, LSL #2]; \
	ldr		ctask,  [cpu_struct, #CPUSTRUCT_CTASK];


	.text
	.balign 4

/**
** _dispatch
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
**		import _ctask	(current task)
**		import _ntask	(next task)
*/
	.global _dispatch
	.type   _dispatch, %function

_dispatch:
	/* ※ 関数として呼ばれるエントリなので R0/R1/R2/R3/R12は破壊されても良い */
	push	{lr}

	GET_CTASK(r0,lr)
	/* R0:cpu_struck  R1:ctask */

	/* IF _ctask not available ignore save_context */
	cmp		lr, #0
	beq		_restore_context

_save_and_restore:
	/* save _ctask state */
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
	vmrs	r2, FPSCR
	str		r2, [r1]

_no_save_vfp:
	/* save USR/SYS info */
	mrs		r1, SPSR_svc
	mrs		r2, SP_usr
	mrs		r3, LR_usr
	push	{r1,r2,r3}

	str		sp, [lr,#SAVE_SP]

_restore_context:
	/* All register done saved */

	/* update ctask<-ntask and return new ctask */
	/* 全レジスタは退避済み SPは現在の値が一応使える */
	/* R0:cpu_struct */
	bl		_update_and_get_ctask

	mov		lr, r0	/* LR <- new ctask */

	/* check new ctask */
	cmp		lr, #0
	beq		_idle_loop

	/* restore SP (LR is new ctask) */
	ldr		sp, [lr,#SAVE_SP]

	/* restore USR/SYS info */
	pop		{r1,r2,r3}
	msr		SPSR_svc, r1
	msr		SP_usr, r2
	msr		LR_usr, r3

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
	/* ※ R0/R1/R2/R3/R12/LRはすでに退避されているので破壊されても良い */

	GET_CTASK(r0,lr)
	ldr		r1,  [r0, #CPUSTRUCT_NTASK]
	/* R0:cpu_struck  R1:_ntask  LR:_ctask */

	cmp		r1, lr
	beq		_nodispatch_irq /* _ntask == _ctask ? */

	/* _ctask check */
	cmp		lr, #0
	beq		_restore_context /* _ctask is NULL ? */

	/* save current task context */
	ldr		r1, =_entry_stub
	push	{r1}
	b		_save_and_restore

_nodispatch_irq:
	b		_entry_stub

/*
**	_idle_loop
*/
	.type   _idle_loop, %function
_idle_loop:
	cps		#MODE_SVC
	ldr		r0, =stack_idle
	CPUID_get(r1)
	sub		r0, r0, r1, LSR #IDLE_TASK_STACK_SIZE_SHIFT
	mov		sp, r0
1:
	bl		idle_task
	b		1b
