/*
 * Copyright (C) 2015 syujibiwa All rights reserved.
 */

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
	Link			link;
#if USE_SMP==1
	SpinLockObj		spin_lock;		/* spin lock */
#endif
	bool			id_initialized;	/* 初期完了フラグ */
	uint32_t		max;			/* 全リソース個数 */
	uint32_t		remain;			/* リソース残数 */
} SemStruct;


#endif /* _SEM_H_ */
