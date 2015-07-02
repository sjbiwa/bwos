	.syntax unified
	.thumb
	.thumb_func

/*
** emos boot.S
*/
#include "arm.h"

/* Vector */
	.text
	.balign	4

	.globl	_reset_entry
	.type   _reset_entry, %function
_reset_entry:

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

/* stack for IDLE-TASK */
	.section .stack
	.balign 16
	.globl	stack_idle
	.space	IDLE_TASK_STACK_SIZE * CPU_NUM
stack_idle:

/* MSP(割り込みハンドラ)で使うスタック */
	.balign	16
	.globl	stack_master
	.space	IRQ_STACK_SIZE * CPU_NUM
stack_master:
