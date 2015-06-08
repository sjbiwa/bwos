/*
 * sem.h
 *
 *  Created on: 2015/01/04
 *      Author: biwa
 */

#ifndef _SEM_H_
#define _SEM_H_

#include "common.h"
#include "link.h"
#include "smp.h"

typedef	struct tagSemStruct {
	Link		link;
	SpinLockObj	spin_lock;		/* spin lock */
	uint32_t	max;			/* 全リソース個数 */
	uint32_t	remain;			/* リソース残数 */
} SemStruct;


#endif /* _SEM_H_ */
