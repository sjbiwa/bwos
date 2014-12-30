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
#include "malloc.h"

#define	RT_OK				(0)
#define	RT_ERR				(-1)
#define	RT_TIMEOUT			(-2)
#define	RT_WAKEUP			(-3)
#define	RT_EVENT			(-4)

/* タスク関連API */
OSAPI int task_create(TaskStruct* task);
OSAPI void task_sleep(void);
OSAPI void task_wakeup(TaskStruct* task);
OSAPI int32_t task_tsleep(TimeSpec tm);
OSAPI void* task_get_tls(TaskStruct* task);

/* フラグ関連API */
OSAPI int flag_create(FlagStruct* flag);
OSAPI void flag_set(FlagStruct* flag);
OSAPI int flag_wait(FlagStruct* flag, TimeSpec tmout);

/* ミューテックス関連API */
OSAPI int mutex_create(MutexStruct* mtx);
OSAPI int mutex_unlock(MutexStruct* mtx);
OSAPI int mutex_lock(MutexStruct* mtx, TimeSpec tmout);

/* ヒープメモリ関連API (名前は再検討) */
OSAPI void* sys_malloc(uint32_t size);
OSAPI void sys_free(void* ptr);
OSAPI void* sys_malloc_align(uint32_t size, uint32_t align);


void* __sys_malloc(uint32_t size);
void* __sys_malloc_align(uint32_t size, uint32_t align);

#endif /* INC_API_H_ */
