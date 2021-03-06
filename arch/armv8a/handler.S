/*
 * Copyright (C) 2015 sjbiwa All rights reserved.
 */
#include "armv8.h"
#include "armv8reg.h"
#include "gicv3reg.h"

#define	STACK_INIT(sp_base,cpuid)		ldr	x0, =(sp_base); \
										sub x0, x0, cpuid, LSL #EXCEPTION_STACK_SIZE_SHIFT; \
										mov sp, x0

#define	LED_ON(v)						ldr x8, =0x1C010008; mov w9, ##v; str w9, [x8]

#define	ENAINT_IN_ISR


.macro VECTOR_ENTRY func
	.align	7
	stp		x18, x30, [sp, #-16]!
	stp		x16, x17, [sp, #-16]!
	stp		x14, x15, [sp, #-16]!
	stp		x12, x13, [sp, #-16]!
	stp		x10, x11, [sp, #-16]!
	stp		 x8,  x9, [sp, #-16]!
	stp		 x6,  x7, [sp, #-16]!
	stp		 x4,  x5, [sp, #-16]!
	stp		 x2,  x3, [sp, #-16]!
	stp		 x0,  x1, [sp, #-16]!
	ELR_EL1_get(x8)
	SPSR_EL1_get(x9)
	stp		 x8,  x9, [sp, #-16]!
	b		\func
.endm

.macro	VECTOR_RETURN
	ldp		 x8,  x9, [sp], #16
	ELR_EL1_set(x8)
	SPSR_EL1_set(x9)
	ldp		 x0,  x1, [sp], #16
	ldp		 x2,  x3, [sp], #16
	ldp		 x4,  x5, [sp], #16
	ldp		 x6,  x7, [sp], #16
	ldp		 x8,  x9, [sp], #16
	ldp		x10, x11, [sp], #16
	ldp		x12, x13, [sp], #16
	ldp		x14, x15, [sp], #16
	ldp		x16, x17, [sp], #16
	ldp		x18, x30, [sp], #16
	eret
.endm

.macro VECTOR_ENTRY_F func
	.align	7
	b		\func
.endm

	.section .text
	.text
	.global	_exc_vector
	.type	_exc_vector, %function
_exc_vector:
	.align	11
	/* from EL1 with SP_EL0 (invalid) */
	VECTOR_ENTRY_F		invalid_handler
	VECTOR_ENTRY_F		invalid_handler
	VECTOR_ENTRY_F		invalid_handler
	VECTOR_ENTRY_F		invalid_handler
	/* from EL1 with SP_EL1 */
	VECTOR_ENTRY		el1_sync_handler
	VECTOR_ENTRY		el1_irq_handler
	VECTOR_ENTRY		el1_fiq_handler
	VECTOR_ENTRY		el1_serr_handler
	/* from EL0 */
	VECTOR_ENTRY		el0_sync_handler
	VECTOR_ENTRY		el0_irq_handler
	VECTOR_ENTRY		el0_fiq_handler
	VECTOR_ENTRY		el0_serr_handler
	/* from AArch32 (invalid) */
	VECTOR_ENTRY_F		invalid_handler
	VECTOR_ENTRY_F		invalid_handler
	VECTOR_ENTRY_F		invalid_handler
	VECTOR_ENTRY_F		invalid_handler

el0_irq_handler:
el1_irq_handler:
	/* IRQネスト値更新 */
	CPUID_get(x0)
	ldr		x1, =_irq_level
	add		x1, x1, x0, LSL #2
	ldr		w2, [x1]
	add		w0, w2, #1
	str		w0, [x1]
	/* ネスト値==1(first-in)? */
	cbnz	w2, enter_from_handler

enter_from_task:
	CPACR_EL1_get(x0)
	CPACR_EL1_set(xzr)
	isb
	sub		sp, sp, #16
	str		x0, [sp, #0]
	mov		x8, sp
	CPUID_get(x2)
	/* IRQスタックに切り替え/SYSTEMスタック保存 */
	STACK_INIT(stack_irq_base,x2)
	sub		sp, sp, #16
	str		x8, [sp, #0]
enter_from_handler:

	mov		x0, x8
	ELR_EL1_get(x1)
	SPSR_EL1_get(x2)

	bl		c_handler

	/* IRQネスト値更新 */
	CPUID_get(x0)
	ldr		x1, =_irq_level
	add		x1, x1, x0, LSL #2
	ldr		w2, [x1]
	sub		w2, w2, #1
	str		w2, [x1]
	/* ネスト値==0(return to task)? */
	cbnz	w2, ret_to_handler

ret_to_task:
	ldr		x1, [sp, #0]
	mov		sp, x1
	/* CPACRをとりあえず復帰 */
	ldr		x1, [sp, #0]
	CPACR_EL1_set(x1)
	isb
	add		sp, sp, #16

	/* check _dispatch_disable */
	ldr		x1, =_dispatch_disable
	ldrb	w2, [x1,x0]
	cbnz	w2, ret_with_nodispatch

	b		_dispatch_irq

ret_to_handler:
ret_with_nodispatch:
	b		_nodispatch_irq


el0_sync_handler:
el1_sync_handler:
	ESR_EL1_get(x8)
	and		w9, w8, #0xFC000000
	lsr		w9, w9, #26
	cmp		w9, #0x15	 /* check SVC? */
	bne		general_sync_handler
svc_handler:
	/* X0-X7までは引数なので使用しないこと */
	/* x8:ESR_EL1 */

	/* ベクタエントリ取得 */
	and		x9, x8, #0xffff
	lsl		x9, x9, #3
	ldr		x10, =svc_entry_table
	ldr		x10, [x10, x9]

#if defined(ENAINT_IN_ISR)
	/* 呼び出し時のDAIFを継承する */
	SPSR_EL1_get(x9)
	lsr		w9, w9, #6
	and		w9, w9, #0xf
	DAIF_set(x9)
#endif

	blr		x10

#if defined(ENAINT_IN_ISR)
	/* 割り込み禁止 */
	DAIFSet_seti(0xf)
#endif

	/* X0をスタック上のX0に格納 */
	str		x0, [sp, #2*8]

	VECTOR_RETURN


general_sync_handler:
	stp		x29, x29, [sp, #-16]!
	stp		x27, x28, [sp, #-16]!
	stp		x25, x26, [sp, #-16]!
	stp		x23, x24, [sp, #-16]!
	stp		x21, x22, [sp, #-16]!
	stp		x19, x20, [sp, #-16]!
	mov		x0, sp
	bl		general_exception_handler

el0_fiq_handler:
	mov		x0, #1
	b		run_c_exc_handler
	
el0_serr_handler:
	mov		x0, #2
	b		run_c_exc_handler
	
el1_fiq_handler:
	mov		x0, #3
	b		run_c_exc_handler
	
el1_serr_handler:
	mov		x0, #4
	b		run_c_exc_handler
	
invalid_handler:
	mov		x0, #5

run_c_exc_handler:
	bl		c_exc_handler
	ret


	.section .stack

	.balign	16
	.space	IRQ_STACK_SIZE * CPU_NUM
stack_irq_base:
