/*
 * flag.h
 *
 *  Created on: 2014/11/18
 *      Author: biwa
 */

#ifndef FLAG_H_
#define FLAG_H_

#include "common.h"
#include "link.h"
#include "smp.h"

typedef	struct tagFlagStruct {
	Link			link;
	SpinLockObj		spin_lock;		/* spin lock */
	bool			id_initialized;	/* 初期完了フラグ */
	uint32_t		value;
} FlagStruct;


#endif /* FLAG_H_ */
