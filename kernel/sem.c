/*
 * sem.c
 *
 *  Created on: 2015/01/04
 *      Author: biwa
 */

#include "common.h"
#include "task.h"
#include "sem.h"
#include "link.h"
#include "api.h"

typedef	struct {
	SemStruct*		obj;	/* セマフォオブジェクト */
	uint32_t		count;	/* セマフォ要求カウント */
} SemInfoStruct;

static void sem_release_body(SemStruct* sem)
{
	/* linkの先頭タスクをチェック */
	while ( !link_is_empty(&(sem->link)) ) {
		Link* link = sem->link.next;
		TaskStruct* task = containerof(link, TaskStruct, link);
		SemInfoStruct* sem_info = (SemInfoStruct*)task->wait_obj;
		/* 要求数をチェック */
		if ( sem->remain < sem_info->count ) {
			break;
		}
		/* 要求数を満たすのでタスク起床 */
		sem->remain -= sem_info->count;
		link_remove(link);
		task_wakeup_stub(task, RT_OK);
	}
}

/* セマフォ待ちのタスクがタイムアウトした場合 */
static void sem_wait_func(TaskStruct* task)
{
	SemStruct* sem = (SemStruct*)task->wait_obj;
	sem_release_body(sem);
}

OSAPI int sem_create(SemStruct* sem, uint32_t max)
{
	link_clear(&(sem->link));
	sem->max = sem->remain = max;
	return 0;
}

OSAPI int sem_trequest(SemStruct* sem, uint32_t num, TimeOut tmout)
{
	uint32_t		cpsr;
	irq_save(cpsr);

	/* リターンコード設定 */
	_ctask->result_code = RT_OK;

	if ( (num <= sem->remain) && link_is_empty(&(sem->link)) ) {
		/* すぐに確保できる場合 */
		sem->remain -= num;
	}
	else {
		/* 先に待ちタスクがいるか数が足りない場合は待ちに入る */
		SemInfoStruct sem_info;
		sem_info.obj = sem;
		sem_info.count = num;
		_ctask->wait_obj = (void*)&sem_info;
		_ctask->wait_func = sem_wait_func;
		task_remove_queue(_ctask);
		_ctask->task_state = TASK_WAIT;
		link_add_last(&(sem->link), &(_ctask->link));
		if ( tmout != TMO_FEVER ) {
			/* タイムアウトリストに追加 */
			task_add_timeout(_ctask, tmout);
		}
		schedule();
	}
	irq_restore(cpsr);
	return _ctask->result_code;
}

OSAPI int sem_request(SemStruct* sem, uint32_t num)
{
	return sem_trequest(sem, num, TMO_FEVER);
}

OSAPI int sem_release(SemStruct* sem, uint32_t num)
{
	uint32_t		cpsr;
	int				ret = RT_OK;
	irq_save(cpsr);

	sem->remain += num;
	if ( sem->max < sem->remain ) {
		sem->remain = sem->max;
	}

	sem_release_body(sem);
	schedule();

	irq_restore(cpsr);
	return ret;
}
