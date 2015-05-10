/*
 * mutex.c
 *
 *  Created on: 2014/11/15
 *      Author: biwa
 */

#include "kernel.h"

/* オブジェクト<->インデックス変換用 */
OBJECT_INDEX_FUNC(mutex,MutexStruct,MUTEX_MAX_NUM);

int _kernel_mutex_create(MutexStruct* mtx)
{
	link_clear(&(mtx->link));
	mtx->task = NULL;
	mtx->count = 0;
	return RT_OK;
}

int _kernel_mutex_unlock(MutexStruct* mtx)
{
	uint32_t		cpsr;
	int				ret = RT_OK;
	irq_save(cpsr);
	if ( mtx->task == _ctask ) {
		mtx->count--;
		if ( mtx->count == 0 ) {
			mtx->task = NULL;
			if ( !link_is_empty(&(mtx->link)) ) {
				/* linkの先頭タスクをwakeup */
				Link* link = mtx->link.next;
				link_remove(link);
				TaskStruct* task = containerof(link, TaskStruct, link);
				task_wakeup_stub(task, RT_OK);
				/* 起床したタスクにmutexの所有権設定 */
				mtx->task = task;
				mtx->count = 1;
				schedule();
			}
		}
	}
	else {
		/* lockタスクと違うタスクがunlock要求を出したのでエラー */
		ret = RT_ERR;
	}
	irq_restore(cpsr);
	return ret;
}

int _kernel_mutex_tlock(MutexStruct* mtx, TimeOut tmout)
{
	uint32_t		cpsr;
	irq_save(cpsr);

	/* リターンコード設定 */
	_ctask->result_code = RT_OK;

	if ( mtx->task == NULL ) {
		/* lock中のタスクが無ければ自タスクのlockは成功 */
		mtx->task = _ctask;
		mtx->count = 1;
	}
	else if ( mtx->task == _ctask ) {
		/* 自タスクでlock中ならカウントアップのみ */
		mtx->count++;
	}
	else if ( tmout == TMO_POLL ) {
		/* ポーリングなのでタイムアウトエラーとする */
		_ctask->result_code = RT_TIMEOUT;
	}
	else {
		/* 他タスクlock中なので待ち状態に遷移 */
		task_set_wait(_ctask, 0, 0);
		task_remove_queue(_ctask);
		link_add_last(&(mtx->link), &(_ctask->link));
		if ( tmout != TMO_FEVER ) {
			/* タイムアウトリストに追加 */
			task_add_timeout(_ctask, tmout);
		}
		schedule();
	}
	irq_restore(cpsr);
	return _ctask->result_code;
}

int _kernel_mutex_lock(MutexStruct* mtx)
{
	return _kernel_mutex_tlock(mtx, TMO_FEVER);
}

OSAPISTUB int __mutex_create(void)
{
	int ret = RT_ERR;
	int mutex_id = alloc_mutex_id();
	if ( 0 <= mutex_id ) {
		MutexStruct* mutex = mutexid2object(mutex_id);
		ret = _kernel_mutex_create(mutex);
		if ( ret == RT_OK ) {
			ret = mutex_id;
		}
		else {
			free_mutex_struct(mutex_id);
		}
	}
	return ret;
}

OSAPISTUB int __mutex_unlock(int id)
{
	MutexStruct* mutex = mutexid2object(id);
	return _kernel_mutex_unlock(mutex);
}

OSAPISTUB int __mutex_lock(int id)
{
	MutexStruct* mutex = mutexid2object(id);
	return _kernel_mutex_lock(mutex);
}

OSAPISTUB int __mutex_tlock(int id, TimeOut tmout)
{
	MutexStruct* mutex = mutexid2object(id);
	return _kernel_mutex_tlock(mutex, tmout);
}
