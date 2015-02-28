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
#include "kernel_api.h"

typedef	struct {
	SemStruct*		sem;	/* セマフォオブジェクト */
	uint32_t		count;	/* セマフォ要求カウント */
} SemInfoStruct;

/* オブジェクト<->インデックス変換用 */
OBJECT_INDEX_FUNC(sem,SemStruct,SEM_MAX_NUM);


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
		task->wait_func = 0; /* 再度同じ処理が走らないようにクリアする */
		task_wakeup_stub(task, RT_OK);
	}
}

/* セマフォ待ちのタスクがタイムアウトした場合 */
static void sem_wait_func(TaskStruct* task)
{
	SemStruct* sem = ((SemInfoStruct*)task->wait_obj)->sem;
	sem_release_body(sem);
}


int _kernel_sem_create(SemStruct* sem, uint32_t max)
{
	link_clear(&(sem->link));
	sem->max = sem->remain = max;
	return RT_OK;
}

int _kernel_sem_trequest(SemStruct* sem, uint32_t num, TimeOut tmout)
{
	uint32_t		cpsr;
	irq_save(cpsr);

	/* リターンコード設定 */
	_ctask->result_code = RT_OK;

	if ( (num <= sem->remain) && link_is_empty(&(sem->link)) ) {
		/* すぐに確保できる場合 */
		sem->remain -= num;
	}
	else if ( tmout == TMO_POLL ) {
		/* ポーリングなのでタイムアウトエラーとする */
		_ctask->result_code = RT_TIMEOUT;
	}
	else {
		/* 先に待ちタスクがいるか数が足りない場合は待ちに入る */
		SemInfoStruct sem_info;
		sem_info.sem = sem;
		sem_info.count = num;
		task_remove_queue(_ctask);
		task_set_wait(_ctask, (void*)&sem_info, sem_wait_func);
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

int _kernel_sem_request(SemStruct* sem, uint32_t num)
{
	return __sem_trequest(sem, num, TMO_FEVER);
}

int _kernel_sem_release(SemStruct* sem, uint32_t num)
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

OSAPISTUB int __sem_create(uint32_t max)
{
	int ret = RT_ERR;
	int sem_id = alloc_sem_id();
	if ( 0 <= sem_id ) {
		SemStruct* sem = semid2object(sem_id);
		ret = _kernel_sem_create(sem, max);
		if ( ret == RT_OK ) {
			ret = sem_id;
		}
		else {
			free_sem_struct(sem_id);
		}
	}
	return ret;
}

OSAPISTUB int __sem_request(int id, uint32_t num)
{
	SemStruct* sem = semid2object(id);
	return _kernel_sem_request(sem, num);
}

OSAPISTUB int __sem_trequest(int id, uint32_t num, TimeOut tmout)
{
	SemStruct* sem = semid2object(id);
	return _kernel_sem_trequest(sem, num, tmout);
}

OSAPISTUB int __sem_release(int id, uint32_t num)
{
	SemStruct* sem = semid2object(id);
	return _kernel_sem_release(sem, num);
}
