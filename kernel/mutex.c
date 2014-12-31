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
}

OSAPI int mutex_lock(MutexStruct* mtx, TimeOut tmout)
{
	(void)tmout; /* 現在は未実装 */
	uint32_t		cpsr;
	_ctask->result_code = RT_OK;
	irq_save(cpsr);
	if ( mtx->task == NULL ) {
		/* lock中のタスクが無ければ自タスクのlockは成功 */
		mtx->task = _ctask;
		mtx->count = 1;
	}
	else if ( mtx->task == _ctask ) {
		/* 自タスクでlock中ならカウントアップのみ */
		mtx->count++;
	}
	else {
		/* 他タスクlock中なので待ち状態に遷移 */
		task_remove_queue(_ctask);
		_ctask->task_state = TASK_WAIT;
		link_add_last(&(mtx->link), &(_ctask->link));
		schedule();
	}
	irq_restore(cpsr);
	return _ctask->result_code;
}
