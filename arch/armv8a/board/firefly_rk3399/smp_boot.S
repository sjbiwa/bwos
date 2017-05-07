/*
 * Copyright (C) 2016 sjbiwa All rights reserved.
 */
#include "armv8.h"
#include "armv8reg.h"

	.align	2
	.text
	.global	_smp_boot
	.type	_smp_boot, %function
_smp_boot:
	smc	#0
	ret
