/*
 * Copyright (C) 2016 sjbiwa All rights reserved.
 */

/*
 * arch_smp.h
 *
 *  Created on: 2016/01/11
 *      Author: biwa
 */

#ifndef _ARCH_SMP_H_
#define _ARCH_SMP_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef	struct {
	uint16_t	owner;		/* 獲得中のID */
	uint16_t	req;		/* リクエスタID */
	uint32_t	cpuid;		/* 獲得中のCPUID */
	uint32_t	lr;			/* 獲得中処理のLR */
} SpinLockObj;

#ifdef __cplusplus
}
#endif

#endif /* _ARCH_SMP_H_ */
