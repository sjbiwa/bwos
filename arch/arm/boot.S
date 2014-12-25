	.syntax unified
	.cpu cortex-a9
/*
** emos boot.S
*/
#include "arm.h"
#include "cp15reg.h"

#define	STACK_INIT(cpu_mode,mode_sp)		cps #(cpu_mode);ldr sp, =(mode_sp)

#define	SCTLR_TE	(0x01<<30)		/* thumb exception enable */
#define	SCTLR_I		(0x01<<12)		/* instruction cache enable */
#define	SCTLR_Z		(0x01<<11)		/* brunch prediction enable */
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
	ldr		r1, =(SCTLR_TE|SCTLR_I|SCTLR_Z)
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

	.type   _vector, %function
	.balign 4
_vector:
	ldr		pc, _reset_vector
	.balign 4
	ldr		pc, _undef_vector
	.balign 4
	ldr		pc, _swi_vector
	.balign 4
	ldr		pc, _pref_vector
	.balign 4
	ldr		pc, _dabort_vector
	.balign 4
	ldr		pc, _non_vector
	.balign 4
	ldr		pc, _irq_vector
	.balign 4
	ldr		pc, _fiq_vector

_reset_vector:
	.word	_reset_entry
_undef_vector:
	.word	_undef_entry
_swi_vector:
	.word	_swi_entry
_pref_vector:
	.word	_pref_entry
_dabort_vector:
	.word	_dabort_entry
_non_vector:
	.word	0
_irq_vector:
	.word	_irq_entry
_fiq_vector:
	.word	_fiq_entry


	.text
/* Reset Handler */
	.type   _reset_entry, %function
_reset_entry:

	/* CBAR read */
	MRC p15, 4, r0, c15, c0, 0
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
