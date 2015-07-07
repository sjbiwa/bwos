	.syntax unified
	.thumb
	.thumb_func

/*
** bwos boot.S
*/
#include "arm.h"

/* Vector */
	.text
	.balign	4

	.globl	_reset_entry
	.type   _reset_entry, %function
	.globl	_start
	.type   _start, %function
_reset_entry:
_start:
	movs	r0, #1
	msr		PRIMASK, r0
	movs	r0, #0
#if __ARM_ARCH == 7
	msr		FAULTMASK, r0
	msr		BASEPRI, r0
#endif
	msr		CONTROL, r0
	isb
	ldr		r0, =stack_master
	msr		MSP, r0
	ldr		r0, =stack_idle
	msr		PSP, r0

	/* change to PSP for stack */
	movs	r0, #0x2
	msr		CONTROL, r0
	isb

	/* Copy from idata to data */
	ldr		r0, =__idata_start
	ldr		r1, =__data_start
	ldr		r2, =__data_end
	cmp		r0, r1
	beq		_data_done
_data_loop:
#if __ARM_ARCH == 7
	ldr		r3, [r0], #4
	str		r3, [r1], #4
#else
	ldr		r3, [r0]
	str		r3, [r1]
	adds	r0, #4
	adds	r1, #4
#endif
	cmp		r1, r2
	blo		_data_loop
_data_done:

	/* bss clear */
	ldr		r0, =__bss_start
	ldr		r1, =__bss_end
	movs	r2, #0
_bss_loop:
	cmp		r0, r1
	bhs		_bss_done
#if __ARM_ARCH == 7
	str		r2, [r0], #4
#else
	str		r2, [r0]
	adds	r0, #4
#endif
	b		_bss_loop
_bss_done:

	bl		system_init

	bl		startup

_loop_:
	b		_loop_


/* stack for IDLE-TASK */
	.section .stack
	.balign 16
	.globl	stack_idle
	.space	IDLE_TASK_STACK_SIZE
stack_idle:

/* MSP(割り込みハンドラ)で使うスタック */
	.balign	16
	.globl	stack_master
	.space	IRQ_STACK_SIZE
stack_master:
