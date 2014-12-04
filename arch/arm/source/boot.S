	.syntax unified
	.cpu cortex-a9
/*
** emos boot.S
*/
#include "common.h"
#include "arm.h"
#include "cp15reg.h"

#define	STACK_INIT(cpu_mode,mode_sp)		cps #(cpu_mode);ldr sp, =(mode_sp)


/* Vector */
	.arm
	.text
	.balign	4

	.section	.vector
	.type   _start, %function
	.globl	_start
_start:
	/* set SCTLR.TE */
	SCTLR_get(r0)
	mov		r1, #(0x01<<30)
	orr		r0, r0, r1
	SCTLR_set(r0)
	isb
	/* switch to THUMB(Jump to Reset Vector) */
	ldr		r0, =_vector
	mov		r1, #0x01
	orr		r0, r0, r1
	bx		r0

	.thumb
	.thumb_func
	.text
	.balign	32
_vector:
	ldr		pc, _reset_vector
	ldr		pc, _undef_vector
	ldr		pc, _swi_vector
	ldr		pc, _pref_vector
	ldr		pc, _dabort_vector
	ldr		pc, _non_vector
	ldr		pc, _irq_vector
	ldr		pc, _fiq_vector

_reset_vector:
	.word	_reset_entry+1
_undef_vector:
	.word	_undef_entry+1
_swi_vector:
	.word	_swi_entry+1
_pref_vector:
	.word	_pref_entry+1
_dabort_vector:
	.word	_dabort_entry+1
_non_vector:
	.word	0
_irq_vector:
	.word	_irq_entry+1
_fiq_vector:
	.word	_fiq_entry+1


	.text
/* Reset Handler */
_reset_entry:
	/* VectorBase setting */
	ldr		r0, =_vector
	VBAR_set(r0)

	/* Initialize StackPointer */
	STACK_INIT(MODE_UND, stack_undef)
	STACK_INIT(MODE_ABT, stack_dabort)
	STACK_INIT(MODE_IRQ, stack_irq)
	STACK_INIT(MODE_FIQ, stack_fiq)
	STACK_INIT(MODE_SYS, stack_sys)
	STACK_INIT(MODE_SVC, stack_svc)

	/* Copy from idata to data */
	ldr		r0, =__idata_start
	ldr		r1, =__data_start
	ldr		r2, =__data_end
	cmp		r0, r1
	beq		_data_done
_data_loop:
	ldr		r3, [r0], #4
	str		r3, [r1], #4
	cmp		r1, r2
	bne		_data_loop
_data_done:

	/* bss clear */
	ldr		r0, =__bss_start
	ldr		r1, =__bss_end
	mov		r2, #0
_bss_loop:
	cmp		r0, r1
	beq		_bss_done
	str		r2, [r0], #4
	b		_bss_loop
_bss_done:

	bl		main

_loop_:
	b		_loop_

/* Stack Area */
	.section	.stack
	.balign	16
	.space	1024
stack_undef:

	.balign	16
	.space	1024
stack_svc:

	.balign	16
	.space	1024
stack_sys:

	.balign	16
	.space	1024
stack_dabort:

	.balign	16
	.space	1024
stack_irq:

	.balign	16
	.space	1024
stack_fiq:
