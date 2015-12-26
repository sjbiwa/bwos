/*
 * Copyright (C) 2015 sjbiwa All rights reserved.
 */
#include "armv8.h"
#include "armv8reg.h"
#include "gicv3reg.h"

.macro IRQ_VECTOR_ENTRY func
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
	ELR_EL1_get(x0)
	SPSR_EL1_get(x1)
	stp		 x0,  x1, [sp, #-16]!
	b		\func
.endm

.macro	VECTOR_RETURN
	ldp		 x0,  x1, [sp], #16
	ELR_EL1_set(x0)
	SPSR_EL1_set(x1)
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
_exc_vector:
	.align	11
	/**/
	VECTOR_ENTRY		sync_handler
	IRQ_VECTOR_ENTRY	irq_handler
	VECTOR_ENTRY		fiq_handler
	VECTOR_ENTRY		serr_handler
	/**/
	VECTOR_ENTRY		sync_handler
	IRQ_VECTOR_ENTRY	irq_handler
	VECTOR_ENTRY		fiq_handler
	VECTOR_ENTRY		serr_handler
	/**/
	VECTOR_ENTRY		sync_handler
	IRQ_VECTOR_ENTRY	irq_handler
	VECTOR_ENTRY		fiq_handler
	VECTOR_ENTRY		serr_handler
	/**/
	VECTOR_ENTRY		sync_handler
	IRQ_VECTOR_ENTRY	irq_handler
	VECTOR_ENTRY		fiq_handler
	VECTOR_ENTRY		serr_handler

irq_handler:
	/* IRQネスト値更新 */
	IRQLEVEL_inc(w0)
	cmp		w0, #1
	bne		enter_from_handler

enter_from_task:
	CPACR_EL1_get(x0)
	CPACR_EL1_set(xzr)
	sub		sp, #16
	str		x0, [sp, #0]
	mov		x0, sp
	CPUID_get(x2)
	/* IRQスタックに切り替え/SYSTEMスタック保存 */
	STACK_INIT(EL1,stack_irq_base,x2)
	sub		sp, #16
	str		x0, [sp, #0]
enter_from_handler:
enter_op_done:

	bl		c_irq_handler

	/* IRQネスト値更新 */
	IRQLEVEL_dec(w0)
	cmp		w0, #0
	bne		ret_to_handler
ret_to_task:
	ldr		sp, [sp, #0]

	/* 遅延ディスパッチチェック */

	/* IF 要dispatch? */
	beq		delayed_dispatch

ret_to_handler:
	VECTOR_RETURN

delayed_dispatch:
	/* 遅延ディスパッチ処理 */

sync_handler:
fiq_handler:
serr_handler:
exc_handler:
	bl		c_exc_handler
	ret

	.section .stack

	.balign	16
	.space	EXCEPTION_STACK_SIZE * CPU_NUM
stack_el1_base:

	.balign	16
	.space	IRQ_STACK_SIZE * CPU_NUM
stack_irq_base:
