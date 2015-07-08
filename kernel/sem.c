/*
 * sem.c
 *
 *  Created on: 2015/01/04
 *      Author: biwa
 */

#include "kernel.h"

typedef	struct {
	SemStruct*		sem;	/* セマフォオブジェクト */
	uint32_t		count;	/* セマフォ要求カウント */
} SemInfoStruct;

/* オブジェクト<->インデックス変換用 */
static void sem_sub_init(void) {}
OBJECT_INDEX_FUNC(sem,SemStruct,SEM_MAX_NUM);
OBJECT_SPINLOCK_FUNC(sem,SemStruct);
OBJECT_SPINLOCK_FUNC(cpu,CpuStruct);

#if SEM_MAX_NUM != 0

static int sem_release_body(SemStruct* sem, uint32_t num, bool is_context)
{
	uint32_t	irq_state;
	bool		done_update_remain = false;
	uint32_t	wakeup_cpu_list = 0;
	int ret = RT_OK;
retry_lock:
	if ( is_context ) {
		irq_state = irq_save();
	}
	sem_spinlock(sem);

	if ( is_context && !done_update_remain ) {
		/* 最初の１回目に取得値を更新する */
		done_update_remain = true;
		sem->remain += num;
		if ( sem->max < sem->remain ) {
			sem->remain = sem->max;
		}
	}

	/* linkの先頭タスクをチェック */
	while ( !link_is_empty(&(sem->link)) ) {
		Link* link = sem->link.next;
		TaskStruct* task = containerof(link, TaskStruct, link);
		CpuStruct* cpu = task->cpu_struct;
		if ( is_context ) {
			if ( !cpu_spintrylock(cpu) ) {
				/* cpuがlockできなければいったんsem開放して再取得 */
				sem_spinunlock(sem);
				irq_restore(irq_state);
				goto retry_lock;
			}
		}
		else {
			cpu_spinlock(cpu);
		}
		/* sem + cpu をlock完了 */

		SemInfoStruct* sem_info = (SemInfoStruct*)task->wait_obj;
		/* 要求数をチェック */
		if ( sem->remain < sem_info->count ) {
			cpu_spinunlock(cpu);
			break;
		}
		/* 要求数を満たすのでタスク起床 */
		sem->remain -= sem_info->count;
		task_wakeup_stub(task, RT_OK);
		wakeup_cpu_list |= (0x01u << task->cpu_struct->cpuid);
		cpu_spinunlock(cpu);
	}

	sem_spinunlock(sem);

	/* 起床/休止した全タスクに対応するcpuについて再スケジュール */
	wakeup_cpu_list = schedule_any(wakeup_cpu_list);

	if ( USE_SMP == 1 ) {
		/* 自コアは除いておく */
		uint32_t other_cpu_list = wakeup_cpu_list & ~(0x01u<<CPUID_get());

		if ( other_cpu_list ) {
			/* スケジュールされた全コアに 割り込み通知する */
			ipi_request_dispatch(other_cpu_list);
		}
	}

	if ( wakeup_cpu_list & (0x01u<<CPUID_get()) ) {
		self_request_dispatch();
		ret = task_self()->result_code;
	}

	if ( is_context ) {
		irq_restore(irq_state);
	}
	return ret;
}

/* セマフォ待ちのタスクがタイムアウトした場合 */
static void sem_wait_func(TaskStruct* task, void* wait_obj)
{
	SemStruct* sem = ((SemInfoStruct*)wait_obj)->sem;
	CpuStruct* cpu = task->cpu_struct;

	/* タイムアウトしたタスクを起床させる(既に起床している場合は task_wakeup_stubは何もしない) */
	sem_spinlock(sem);
	cpu_spinlock(cpu);
	task_wakeup_stub(task, RT_TIMEOUT);
	cpu_spinunlock(cpu);
	sem_spinunlock(sem);

	/* リソースの待ちリストにつながれているタスクの処理 */
	sem_release_body(sem, 0, false);
}


int _kernel_sem_create(SemStruct* sem, uint32_t max)
{
	link_clear(&(sem->link));
	sem->max = sem->remain = max;
	sem_spininit(sem);
	sync_barrier();

	return RT_OK;
}

int _kernel_sem_trequest(SemStruct* sem, uint32_t num, TimeOut tmout)
{
	SemInfoStruct sem_info;
	uint32_t		irq_state;
	bool req_dispatch = false;
	TaskStruct* task = NULL;
	int ret = RT_OK;

retry_lock:
	irq_state = sem_spinlock_irq_save(sem);

	if ( (num <= sem->remain) && link_is_empty(&(sem->link)) ) {
		/* すぐに確保できる場合 */
		sem->remain -= num;
	}
	else if ( (tmout == TMO_POLL) || !can_dispatch() ) {
		/* ポーリングなのでタイムアウトエラーとする */
		ret = RT_TIMEOUT;
	}
	else {
		/* 先に待ちタスクがいるか数が足りない場合は待ちに入る */
		/* 最初に自cpu lock */
		task = task_self();
		CpuStruct* cpu = task->cpu_struct;
		if ( !cpu_spintrylock(cpu) ) {
			/* cpuがlockできなければいったんsem開放して再取得 */
			sem_spinunlock_irq_restore(sem, irq_state);
			goto retry_lock;
		}
		/* sem + cpu をlock完了 */

		sem_info.sem = sem;
		sem_info.count = num;
		task_remove_queue(task);
		task_set_wait(task, (void*)&sem_info, sem_wait_func);
		link_add_last(&(sem->link), &(task->link));
		if ( tmout != TMO_FEVER ) {
			/* タイムアウトリストに追加 */
			task_add_timeout(task, tmout);
		}
		req_dispatch = schedule(cpu);

		cpu_spinunlock(cpu);
	}

	sem_spinunlock(sem);

	if ( req_dispatch ) {
		self_request_dispatch();
		ret = task->result_code;
	}

	irq_restore(irq_state);
	return ret;
}

int _kernel_sem_request(SemStruct* sem, uint32_t num)
{
	return _kernel_sem_trequest(sem, num, TMO_FEVER);
}

int _kernel_sem_release(SemStruct* sem, uint32_t num)
{
	return sem_release_body(sem, num, true);
}

OSAPISTUB int __sem_create(uint32_t max)
{
	int ret = RT_ERR;
	int sem_id = alloc_sem_id();
	if ( 0 <= sem_id ) {
		SemStruct* sem = semid2buffer(sem_id);
		ret = _kernel_sem_create(sem, max);
		if ( ret == RT_OK ) {
			order_barrier();
			sem->id_initialized = true;
			order_barrier();
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

#else

OSAPISTUB int __sem_create(uint32_t max)
{
	return RT_ERR;
}

OSAPISTUB int __sem_request(int id, uint32_t num)
{
	return RT_ERR;
}

OSAPISTUB int __sem_trequest(int id, uint32_t num, TimeOut tmout)
{
	return RT_ERR;
}

OSAPISTUB int __sem_release(int id, uint32_t num)
{
	return RT_ERR;
}

#endif
