/*
 * api.h
 *
 *  Created on: 2014/11/15
 *      Author: biwa
 */

#ifndef INC_API_H_
#define INC_API_H_

#include <stdint.h>
#include "common.h"
#include "task.h"
#include "flag.h"
#include "mutex.h"
#include "sem.h"
#include "msgq.h"
#include "malloc.h"

#define	RT_OK				(0)
#define	RT_ERR				(-1)
#define	RT_TIMEOUT			(-2)
#define	RT_WAKEUP			(-3)
#define	RT_EVENT			(-4)

/* タスク関連API */
OSAPI int task_create(TaskStruct* task, TaskCreateInfo* info);
OSAPI void task_sleep(void);
OSAPI void task_wakeup(TaskStruct* task);
OSAPI int32_t task_tsleep(TimeOut tm);
OSAPI void* task_get_tls(TaskStruct* task);

/* フラグ関連API */
OSAPI int flag_create(FlagStruct* flag);
OSAPI void flag_set(FlagStruct* flag);
OSAPI int flag_wait(FlagStruct* flag);
OSAPI int flag_twait(FlagStruct* flag, TimeOut tmout);

/* ミューテックス関連API */
OSAPI int mutex_create(MutexStruct* mtx);
OSAPI int mutex_unlock(MutexStruct* mtx);
OSAPI int mutex_lock(MutexStruct* mtx);
OSAPI int mutex_tlock(MutexStruct* mtx, TimeOut tmout);

/* セマフォ関連API */
OSAPI int sem_create(SemStruct* sem, uint32_t max);
OSAPI int sem_request(SemStruct* sem, uint32_t num);
OSAPI int sem_trequest(SemStruct* sem, uint32_t num, TimeOut tmout);
OSAPI int sem_release(SemStruct* sem, uint32_t num);

/* メッセージキュー関連API */
OSAPI int msgq_create(MsgqStruct* msgq, uint32_t length);
OSAPI int msgq_send(MsgqStruct* msgq, void* ptr);
OSAPI int msgq_tsend(MsgqStruct* msgq, void* ptr, TimeOut tmout);
OSAPI int msgq_recv(MsgqStruct* msgq, void** ptr);
OSAPI int msgq_trecv(MsgqStruct* msgq, void** ptr, TimeOut tmout);

/* ヒープメモリ関連API */
OSAPI void* sys_malloc(uint32_t size);
OSAPI void sys_free(void* ptr);
OSAPI void* sys_malloc_align(uint32_t size, uint32_t align);


void* __sys_malloc(uint32_t size);
void* __sys_malloc_align(uint32_t size, uint32_t align);

#endif /* INC_API_H_ */
