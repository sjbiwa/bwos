/*
 * mutex.c
 *
 *  Created on: 2014/11/15
 *      Author: biwa
 */

#include "kernel.h"

/* オブジェクト<->インデックス変換用 */
static void mutex_sub_init(void) {}
OBJECT_INDEX_FUNC(mutex,MutexStruct,MUTEX_MAX_NUM);
OBJECT_SPINLOCK_FUNC(mutex,MutexStruct);
OBJECT_SPINLOCK_FUNC(cpu,CpuStruct);

int _kernel_mutex_create(MutexStruct* mtx)
{
	link_clear(&(mtx->link));
	mtx->locked = false;
	mutex_spininit(mtx);
	sync_barrier();
	return RT_OK;
}

/* タスクがタイムアウトした場合 */
static void mutex_wait_func(TaskStruct* task, void* wait_obj)
{
	MutexStruct* mutex = (MutexStruct*)(wait_obj);
	CpuStruct* cpu = task->cpu_struct;

	/* タイムアウトしたタスクを起床させる(既に起床している場合は task_wakeup_stubは何もしない) */
	mutex_spinlock(mutex);
	cpu_spinlock(cpu);
	task_wakeup_stub(task, RT_TIMEOUT);
	cpu_spinunlock(cpu);
	mutex_spinunlock(mutex);
}

int _kernel_mutex_tlock(MutexStruct* mtx, TimeOut tmout)
{
	uint32_t		irq_state;
	TaskStruct* task = NULL;
	bool req_dispatch = false;
	int ret = RT_OK;

retry_lock:
	irq_state = mutex_spinlock_irq_save(mtx);

	if ( mtx->locked == false ) {
		/* lock中のタスクが無ければ自タスクのlockは成功 */
		mtx->locked = true;
	}
	else if ( tmout == TMO_POLL ) {
		/* ポーリングなのでタイムアウトエラーとする */
		ret = RT_TIMEOUT;
	}
	else {
		/* 他タスクlock中なので待ち状態に遷移 */
		/* 最初に自cpuをlock */
		task = task_self();
		CpuStruct* cpu = task->cpu_struct;
		if ( !cpu_spintrylock(cpu) ) {
			/* cpuがlockできなければいったんmutex開放して再実行 */
			mutex_spinunlock_irq_restore(mtx, irq_state);
			goto retry_lock;
		}
		/* mutex + cpu をlock完了 */

		task_set_wait(task, mtx, mutex_wait_func);
		task_remove_queue(task);
		link_add_last(&(mtx->link), &(task->link));
		if ( tmout != TMO_FEVER ) {
			/* タイムアウトリストに追加 */
			task_add_timeout(task, tmout);
		}
		req_dispatch = schedule(cpu);

		/* cpu開放 */
		cpu_spinunlock(cpu);
	}

	/* mutex開放 */
	mutex_spinunlock(mtx);

	if ( req_dispatch ) {
		self_request_dispatch();
		ret = task->result_code;
	}

	irq_restore(irq_state);
	return ret;
}

int _kernel_mutex_unlock(MutexStruct* mtx)
{
	uint32_t	irq_state;
	int			ret = RT_OK;
	CpuStruct*	req_dispatch_cpu = NULL;

retry_lock:
	irq_state = mutex_spinlock_irq_save(mtx);

	if ( mtx->locked ) {
		if ( !link_is_empty(&(mtx->link)) ) {
			/* linkの先頭タスク取得して対応するCPUをlock */
			Link* link = mtx->link.next;
			TaskStruct* task = containerof(link, TaskStruct, link);
			CpuStruct* cpu = task->cpu_struct;
			if ( !cpu_spintrylock(cpu) ) {
				/* cpuがlockできなければいったんmutex開放して再取得 */
				mutex_spinunlock_irq_restore(mtx, irq_state);
				goto retry_lock;
			}
			/* mutex + cpu をlock完了 */

			/* linkの先頭タスクをwakeupさせる */
			link_remove(link);
			task_wakeup_stub(task, RT_OK);
			/* 起床したタスクが所属するcpuをschedule */
			if ( schedule(cpu) ) {
				req_dispatch_cpu = cpu;
			}

			/* CPU開放 */
			cpu_spinunlock(cpu);
		}
		else {
			mtx->locked = false;
		}
	}
	else {
		/* Lockされていないのでエラー */
		ret = RT_ERR;
	}

	mutex_spinunlock(mtx);

	if ( req_dispatch_cpu != NULL ) {
		if ( req_dispatch_cpu->cpuid == CPUID_get() ) {
			/* 起床したタスクが自CPU所属なので dispatch処理をする */
			self_request_dispatch();
		}
		else {
			/* 起床したタスクが他CPU所属なので 割り込み通知する */
			ipi_request_dispatch_one(req_dispatch_cpu);
		}
	}

	irq_restore(irq_state);
	return ret;
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
