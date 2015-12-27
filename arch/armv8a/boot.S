/*
 * Copyright (C) 2015 sjbiwa All rights reserved.
 */
#include "armv8.h"
#include "armv8reg.h"
#include "gicv3reg.h"

#define	STACK_INIT(exc_level,sp_base,cpuid)		ldr	x0, =(sp_base); \
												sub x0, x0, cpuid, LSL #EXCEPTION_STACK_SIZE_SHIFT; \
												msr SP_##exc_level, x0

	.section .vector
	.align	2
	.global	_start
	.type	_start, %function
_start:
	/* disable interrupt */
	DAIFSet_seti(0xf)
	/* SP is ELx */
	SPSel_seti(1)
	/* get core number */
	CPUID_get(x2)

#if defined(BOOT_FROM_EL3)

	/* initialize EL3 stack */
	STACK_INIT(EL3,stack_el3_base,x2)

	/* EL2/EL3 vector */
	adr		x0, _exc_vector
	VBAR_EL3_set(x0)
	VBAR_EL2_set(x0)

	/* SCR_EL3 / HCR_EL2 */
	ldr		x0, =0x00000400
	SCR_EL3_set(x0)
	ldr		x0, =0x80000000
	HCR_EL2_set(x0)
	CPTR_EL3_set(xzr)
	CPTR_EL2_set(xzr)

	/* timer */
	ldr		x0, =24000000
	CNTFRQ_EL0_set(x0)

	/* CORE==0のみの処理 */
	cmp		x0, #0
	bne		end_gic_setup

	/* init GIC */
	mov		x0, #0x0000000f
	ICC_SRE_EL3_set(x0)
	/* ICC_SRE_EL2設定のため NSへ遷移 */
	SCR_EL3_get(x1)
	orr	x0, x1, #1
	SCR_EL3_set(x0)
	isb
	mov		x0, #0x0000000f
	ICC_SRE_EL2_set(x0)
	SCR_EL3_set(x1)
	isb
	mov		x0, #0x00000007
	ICC_SRE_EL1_set(x0)

	mov		x0, #0x0
	ICC_CTLR_EL3_set(x0)
	mov		x0, #0x3
	ICC_IGRPEN1_EL3_get(x0)
	/**/
end_gic_setup:

	adr		x0, entry_el1
	ELR_EL3_set(x0)
	ldr		x0, =0x000003C5
	SPSR_EL3_set(x0)
	/* goto EL1 entry_el1 */
	eret
#endif

/***************************************/
/* EL1での起動エントリ                 */
/***************************************/
entry_el1:
	/* DAIF disable / SPsel==1 / X2 is CORE-ID */
	/* initialize EL1 stack */
	STACK_INIT(EL1,stack_el1_base,x2)

	/* exception vector base */
	adr		x0, _exc_vector
	VBAR_EL1_set(x0)
	/* FPU/SIMD enable */
	ldr		x0, =0x00300000
	CPACR_EL1_set(x0)

	/* CORE==0のみの処理 */
	cmp		x0, #0
	//bne		_bss_done
	bne		_loop_

	/* Copy from idata to data */
	ldr		x0, =__idata_start
	ldr		x1, =__data_start
	ldr		x2, =__data_end
	cmp		x0, x1
	beq		_data_done
_data_loop:
	ldr		x3, [x0], #8
	str		x3, [x1], #8
	cmp		x1, x2
	blo		_data_loop
_data_done:

	/* bss clear */
	ldr		x0, =__bss_start
	ldr		x1, =__bss_end
	mov		x2, #0
_bss_loop:
	cmp		x0, x1
	bhs		_bss_done
	str		x2, [x0], #8
	b		_bss_loop
_bss_done:

	bl		startup

_loop_:
	wfe
	b		_loop_

	.section .stack

	.balign	16
	.space	EXCEPTION_STACK_SIZE * CPU_NUM
stack_el1_base:

	.balign	16
	.space	EXCEPTION_STACK_SIZE * CPU_NUM
stack_el3_base:
