/*
 * Copyright (C) 2016 sjbiwa All rights reserved.
 */
#include "armv8.h"
#include "armv8reg.h"

	.section .svector
	.align	2
	.global	_sstart
	.type	_sstart, %function
_sstart:
#if USE_SMP == 1
	/* disable interrupt */
	DAIFSet_seti(0xf)
	/* SP is ELx */
	SPSel_seti(1)
	/* get core number */
	CPUID_get(x2)

	/* CORE==MASTER_CPU_ID のみの処理 */
	cmp		x2, #MASTER_CPU_ID
	beq		_cpu_start

	ldr		x0, =smp_boot_flag
_slave_wait:
	wfe
	ldr		w1, [x0]
	cmp		w1, wzr
	bne		_slave_wait
_cpu_start:
#endif
	ldr		x0, =_start
	blr		x0
