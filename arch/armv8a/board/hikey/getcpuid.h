/*
 * Copyright (C) 2016 sjbiwa All rights reserved.
 */

/*
 * getcpuid.h
 *
 *  Created on: 2016/03/01
 *      Author: biwa
 */

#ifndef _GETCPUID_H_
#define _GETCPUID_H_

#include "armv8reg.h"

#ifdef __cplusplus
extern "C" {
#endif

/* コアID取得 */
#ifndef __ASM__

/* コアID取得 */
/* for C/C++ */
#if USE_SMP==1
#define	CPUID_get()			({ uint32_t cpuid = MPIDR_EL1_get() & 0x103; cpuid = (cpuid | (cpuid >> 6)) & 0x7; cpuid;})
#else
#define	CPUID_get()			(0)
#endif

static inline bool is_cluster_master() {
	if ( MPIDR_EL1_get() & 0xff ) {
		return false;
	}
	return true;
}

#else

/* コアID取得 */
/* for ASM */
#if USE_SMP==1
#define	CPUID_get(reg)		MPIDR_EL1_get(reg); \
							and reg, reg, #0x1ff; \
							orr reg, reg, reg, LSR #6; \
							and reg, reg, #0x7
#else
#define	CPUID_get(reg)		mov reg, #0
#endif
#endif

#ifdef __cplusplus
}
#endif

#endif /* MY_BOARD_H_ */
