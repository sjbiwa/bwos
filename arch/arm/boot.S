	.syntax unified
	.cpu cortex-a7
	.thumb
	.thumb_func

/*
** bwos boot.S
*/
#include "arm.h"
#include "cp15reg.h"

#define	STACK_INIT(cpu_mode,mode_sp,cpuid)	ldr r0, =(mode_sp); \
											sub r0, r0, cpuid, LSL #EXCEPTION_STACK_SIZE_SHIFT; \
											msr SP_##cpu_mode, r0

/* Vector */
	.arm
	.text
	.balign	4

	.section	.vector
	.type   _start, %function
	.globl	_start
	.globl	stack_idle
	.globl	stack_intr

_start:
	/* invalidate inst/brnch cache */
	mov		r0, #0
	ICIALLUIS_set(r0)
	BPIALLIS_set(r0)

	/* set SCTLR.TE */
	SCTLR_get(r0)
	ldr		r1, =(SCTLR_TE|SCTLR_I|SCTLR_Z|SCTLR_A)
	orr		r0, r0, r1
	bic		r0, r0, #SCTLR_C
	bic		r0, r0, #SCTLR_M
	bic		r0, r0, #SCTLR_V
	SCTLR_set(r0)
	isb
	ACTLR_get(r0)
	ldr		r1, =ACTLR_SMP
	orr		r0, r0, r1
	ACTLR_set(r0)
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
	ldr		pc, _svc_vector
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
_svc_vector:
	.word	_svc_entry
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
	CPUID_get(r2)
	STACK_INIT(und, stack_undef,r2)
	STACK_INIT(abt, stack_dabort,r2)
	STACK_INIT(irq, stack_irq,r2)
	STACK_INIT(fiq, stack_fiq,r2)
	STACK_INIT(usr, stack_sys,r2)
	STACK_INIT(svc, stack_svc,r2)

	CPUID_get(r2)
	cmp		r2, #0
	bne		_bss_done

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
	blo		_data_loop
_data_done:

	/* bss clear */
	ldr		r0, =__bss_start
	ldr		r1, =__bss_end
	mov		r2, #0
_bss_loop:
	cmp		r0, r1
	bhs		_bss_done
	str		r2, [r0], #4
	b		_bss_loop
_bss_done:

	bl		startup

_loop_:
	b		_loop_

/* Stack Area */
	.section	.stack
	.balign	16
	.space	EXCEPTION_STACK_SIZE * CPU_NUM
stack_undef:

	.balign	16
	.space	EXCEPTION_STACK_SIZE * CPU_NUM
stack_svc:

	.balign	16
	.space	EXCEPTION_STACK_SIZE * CPU_NUM
stack_sys:

	.balign	16
	.space	EXCEPTION_STACK_SIZE * CPU_NUM
stack_dabort:

	.balign	16
	.space	EXCEPTION_STACK_SIZE * CPU_NUM
stack_irq:

	.balign	16
	.space	EXCEPTION_STACK_SIZE * CPU_NUM
stack_fiq:

/* stack for IDLE-TASK */
	.section .stack
	.balign 16
	.space	IDLE_TASK_STACK_SIZE * CPU_NUM
stack_idle:

/* 割り込みハンドラで使うスタック */
	.balign	16
	.space	IRQ_STACK_SIZE * CPU_NUM
stack_intr:
