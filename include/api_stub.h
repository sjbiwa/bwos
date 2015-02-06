/*
 * api_stub.h
 *
 *  Created on: 2014/11/15
 *      Author: biwa
 */

#ifndef INC_API_STUB_H_
#define INC_API_STUB_H_

#include <stdint.h>
#include "common.h"
#include "task.h"
#include "flag.h"
#include "mutex.h"
#include "sem.h"
#include "msgq.h"
#include "fixmb.h"
#include "malloc.h"
#include "api.h"

/* タスク関連API */
OSAPISTUB int __task_create(TaskStruct** task, TaskCreateInfo* info);
OSAPISTUB int __task_active(TaskStruct* task);
OSAPISTUB int __task_sleep(void);
OSAPISTUB int __task_wakeup(TaskStruct* task);
OSAPISTUB int __task_tsleep(TimeOut tm);
OSAPISTUB int __task_get_tls(TaskStruct* task, void** ptr);

/* フラグ関連API */
OSAPISTUB int __flag_create(FlagStruct** flag);
OSAPISTUB int __flag_set(FlagStruct* flag, uint32_t pattern);
OSAPISTUB int __flag_wait(FlagStruct* flag, uint32_t pattern, uint32_t wait_mode, uint32_t* ret_pattern);
OSAPISTUB int __flag_twait(FlagStruct* flag, uint32_t pattern, uint32_t wait_mode, uint32_t* ret_pattern, TimeOut tmout);
OSAPISTUB int __flag_clear(FlagStruct* flag, uint32_t pattern);

/* ミューテックス関連API */
OSAPISTUB int __mutex_create(MutexStruct** mtx);
OSAPISTUB int __mutex_unlock(MutexStruct* mtx);
OSAPISTUB int __mutex_lock(MutexStruct* mtx);
OSAPISTUB int __mutex_tlock(MutexStruct* mtx, TimeOut tmout);

/* セマフォ関連API */
OSAPISTUB int __sem_create(SemStruct** sem, uint32_t max);
OSAPISTUB int __sem_request(SemStruct* sem, uint32_t num);
OSAPISTUB int __sem_trequest(SemStruct* sem, uint32_t num, TimeOut tmout);
OSAPISTUB int __sem_release(SemStruct* sem, uint32_t num);

/* メッセージキュー関連API */
OSAPI int __msgq_create(MsgqStruct** msgq, uint32_t length);
OSAPI int __msgq_send(MsgqStruct* msgq, void* ptr, uint32_t length);
OSAPI int __msgq_tsend(MsgqStruct* msgq, void* ptr, uint32_t length, TimeOut tmout);
OSAPI int __msgq_recv(MsgqStruct* msgq, void* ptr, uint32_t length);
OSAPI int __msgq_trecv(MsgqStruct* msgq, void* ptr, uint32_t length, TimeOut tmout);

/* 固定長メモリブロック関連API */
OSAPISTUB int __fixmb_create(FixmbStruct** fixmb, uint32_t mb_size, uint32_t length);
OSAPISTUB int __fixmb_request(FixmbStruct* fixmb, void** ptr);
OSAPISTUB int __fixmb_trequest(FixmbStruct* fixmb, void** ptr, TimeOut tmout);
OSAPISTUB int __fixmb_release(FixmbStruct* fixmb, void* ptr);

/* ヒープメモリ関連API */
OSAPISTUB void* __sys_malloc(MemSize_t size);
OSAPISTUB void __sys_free(void* ptr);
OSAPISTUB void* __sys_malloc_align(MemSize_t size, uint32_t align);

#endif /* INC_API_STUB_H_ */
