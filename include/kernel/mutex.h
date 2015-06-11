/*
 * mutex.h
 *
 *  Created on: 2014/11/19
 *      Author: biwa
 */

#ifndef MUTEX_H_
#define MUTEX_H_

#include "common.h"
#include "smp.h"
#include "link.h"

typedef	struct tagMutexStruct {
	Link			link;
	SpinLockObj		spin_lock;
	bool			id_initialized;	/* 初期完了フラグ */
	bool			locked;	/* 獲得中？ */
} MutexStruct;

#endif /* MUTEX_H_ */
