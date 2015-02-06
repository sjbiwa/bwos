/*
 * mutex.c
 *
 *  Created on: 2014/11/15
 *      Author: biwa
 */

#include "common.h"
#include "task.h"
#include "mutex.h"
#include "link.h"
#include "api_stub.h"

OSAPISTUB int __mutex_create(MutexStruct** p_mtx)
{
	int ret = RT_ERR;
	MutexStruct* mtx = __sys_malloc_align(sizeof(MutexStruct), NORMAL_ALIGN);
	if ( mtx ) {
		link_clear(&(mtx->link));
		mtx->task = NULL;
		mtx->count = 0;
		*p_mtx = mtx;
		ret = RT_OK;
	}
	return ret;
}

OSAPISTUB int __mutex_unlock(MutexStruct* mtx)
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

OSAPISTUB int __mutex_tlock(MutexStruct* mtx, TimeOut tmout)
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

OSAPISTUB int __mutex_lock(MutexStruct* mtx)
{
	return __mutex_tlock(mtx, TMO_FEVER);
}
