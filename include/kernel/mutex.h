/*
 * Copyright (C) 2015 sjbiwa All rights reserved.
 */

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
#if USE_SMP==1
	SpinLockObj		spin_lock;
#endif
	bool			id_initialized;	/* 初期完了フラグ */
	bool			locked;	/* 獲得中？ */
} MutexStruct;

/* Mutex待ちオブジェクト */
typedef	struct {
	MutexStruct*	mutex;	/* Mutexオブジェクト */
} MutexInfoStruct;

#endif /* MUTEX_H_ */
