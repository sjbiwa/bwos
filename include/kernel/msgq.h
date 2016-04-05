/*
 * Copyright (C) 2015 sjbiwa All rights reserved.
 */

/*
 * msgq.h
 *
 *  Created on: 2015/01/07
 *      Author: biwa
 */

#ifndef MSGQ_H_
#define MSGQ_H_

#include <stdint.h>
#include "common.h"
#include "link.h"

typedef	struct tagMsgqStruct {
	Link			link;
#if USE_SMP==1
	SpinLockObj		spin_lock;			/* spin lock */
#endif
	bool			id_initialized;	/* 初期完了フラグ */
	uint8_t*		data;
	uint32_t		length;
	uint32_t		data_top;
	uint32_t		data_num;
} MsgqStruct;

/* Msgq待ちオブジェクト */
typedef	struct {
	MsgqStruct*		msgq;		/* 要求オブジェクト */
	void*			req_ptr;	/* 要求エリア */
	uint32_t		req_length;	/* 要求バイト数 */
} MsgqInfoStruct;

#endif /* MSGQ_H_ */
