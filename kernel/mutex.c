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
#include "api.h"

OSAPI int mutex_create(MutexStruct* mtx)
{
	link_clear(&(mtx->link));
	mtx->task = NULL;
	mtx->count = 0;
	return 0;
}

OSAPI int mutex_unlock(MutexStruct* mtx)
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

OSAPI int mutex_tlock(MutexStruct* mtx, TimeOut tmout)
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
		_ctask->result_code = RT_OK;
	}
	else {
		/* 他タスクlock中なので待ち状態に遷移 */
		_ctask->wait_obj = 0;
		_ctask->wait_func = 0;
		_ctask->task_state = TASK_WAIT;
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

OSAPI int mutex_lock(MutexStruct* mtx)
{
	return mutex_tlock(mtx, TMO_FEVER);
}
