/*
 * Copyright (C) 2015 sjbiwa All rights reserved.
 */

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
#if USE_SMP==1
	SpinLockObj		spin_lock;		/* spin lock */
#endif
	bool			id_initialized;	/* 初期完了フラグ */
	uint32_t		value;
} FlagStruct;

/* Flag待ちオブジェクト */
typedef	struct {
	FlagStruct*		flag;			/* フラグオブジェクト */
	uint32_t		pattern;		/* waitパターン */
	uint32_t		wait_mode;		/* waitモード */
	uint32_t*		ret_pattern;	/* completeフラグ値 */
} FlagInfoStruct;


#endif /* FLAG_H_ */
