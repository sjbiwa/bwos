/*
 * Copyright (C) 2015 sjbiwa All rights reserved.
 */

/*
 * flag.c
 *
 *  Created on: 2014/11/15
 *      Author: biwa
 */
#include "kernel.h"

typedef	struct {
	FlagStruct*		obj;			/* フラグオブジェクト */
	uint32_t		pattern;		/* waitパターン */
	uint32_t		wait_mode;		/* waitモード */
	uint32_t*		ret_pattern;	/* completeフラグ値 */
} FlagInfoStruct;

/* オブジェクト<->インデックス変換用 */
static void flag_sub_init(void) {}
OBJECT_INDEX_FUNC(flag,FlagStruct,FLAG_MAX_NUM);
OBJECT_SPINLOCK_FUNC(flag,FlagStruct);
OBJECT_SPINLOCK_FUNC(cpu,CpuStruct);

#if FLAG_MAX_NUM != 0

static inline bool is_complete(uint32_t result, uint32_t pattern, uint32_t wait_mode)
{
	bool ret = false;
	if ( wait_mode & FLAG_OR ) {
		if ( (result & pattern) != 0 ) {
			ret = true;
		}
	}
	else if ( wait_mode & FLAG_AND ) {
		if ( result == pattern ) {
			ret = true;
		}
	}
	return ret;
}

static inline bool check_and_result(FlagStruct* flag, uint32_t pattern, uint32_t wait_mode, uint32_t* ret_pattern)
{
	bool ret = false;
	uint32_t result_pattern = flag->value & pattern;
	if ( is_complete(result_pattern, pattern, wait_mode) ) {
		/* completeしたのでフラグクリア処理 */
		if ( wait_mode & FLAG_CLR ) {
			flag->value = 0;
		}
		else if ( wait_mode & FLAG_BITCLR ) {
			flag->value &= ~result_pattern;
		}
		*ret_pattern = result_pattern;
		ret = true;
	}
	return ret;
}

/* タスクがタイムアウトした場合 */
static void flag_wait_func(TaskStruct* task, void* wait_obj)
{
	FlagStruct* flag = ((FlagInfoStruct*)(wait_obj))->obj;
	CpuStruct* cpu = task->cpu_struct;

	/* タイムアウトしたタスクを起床させる */
	flag_spinlock(flag);
	cpu_spinlock(cpu);
	task_wakeup_stub(task, RT_TIMEOUT);
	cpu_spinunlock(cpu);
	flag_spinunlock(flag);
}

int _kernel_flag_create(FlagStruct* flag)
{
	link_clear(&flag->link);
	flag->value = 0;
	flag_spininit(flag);
	sync_barrier();

	return RT_OK;
}

int _kernel_flag_set(FlagStruct* flag, uint32_t pattern)
{
	Link* link;
	uint32_t irq_state;
	uint32_t	wakeup_cpu_list = 0;

	irq_state = flag_spinlock_irq_save(flag);

	flag->value |= pattern;

	/* リストの全タスクをチェック */
	/* flag値が０ならwakeupしないので終了 */
	link = flag->link.next;
	while ( (link != &(flag->link)) && (flag->value != 0) ) {
		TaskStruct* task = containerof(link, TaskStruct, link);
		CpuStruct* cpu = task->cpu_struct;
		cpu_spinlock(cpu);
		link = link->next; /* 現エントリがリストから外される可能性があるので先に次のエントリを取得しておく */
		FlagInfoStruct* flag_info = (FlagInfoStruct*)(task->wait_obj);
		if ( check_and_result(flag, flag_info->pattern, flag_info->wait_mode, flag_info->ret_pattern) ) {
			/* カレントタスクがcomplate */
			task_wakeup_stub(task, RT_OK);
			wakeup_cpu_list |= (0x01u << task->cpu_struct->cpuid);
		}
		cpu_spinunlock(cpu);
	}
	flag_spinunlock(flag);

	/* 起床/休止した全タスクに対応するcpuについて再スケジュール */
	wakeup_cpu_list = schedule_any(wakeup_cpu_list);

	if  ( USE_SMP == 1 ) {
		/* 自コアは除いておく */
		uint32_t other_cpu_list = wakeup_cpu_list & ~(0x01u<<CPUID_get());

		if ( other_cpu_list ) {
			/* スケジュールされた全コアに 割り込み通知する */
			ipi_request_dispatch(other_cpu_list);
		}
	}

	if ( wakeup_cpu_list & (0x01u<<CPUID_get()) ) {
		self_request_dispatch();
	}

	irq_restore(irq_state);
	return RT_OK;
}

int _kernel_flag_twait(FlagStruct* flag, uint32_t pattern, uint32_t wait_mode, uint32_t* ret_pattern, TimeOut tmout)
{
	FlagInfoStruct flag_info;
	uint32_t		irq_state;
	bool req_dispatch = false;
	int				ret = RT_OK;
	TaskStruct*		task;

	irq_state = flag_spinlock_irq_save(flag);

	/* フラグ値とタスクの待ちパターンとで処理する */
	if ( !check_and_result(flag, pattern, wait_mode, ret_pattern) ) {
		if ( (tmout == TMO_POLL) || !can_dispatch() ) {
			/* ポーリングでcompleteしない場合 */
			ret = RT_TIMEOUT;
		}
		else {
			/* completeしない/ポーリングではない場合 */
			task = task_self();
			CpuStruct* cpu = task->cpu_struct;
			cpu_spinlock(cpu);
			/* 待ちリストに追加 */
			flag_info.obj = flag;
			flag_info.pattern = pattern;
			flag_info.wait_mode = wait_mode;
			flag_info.ret_pattern = ret_pattern;
			task_set_wait(task, (void*)(&flag_info), flag_wait_func);
			task_remove_queue(task);
			link_add_last(&(flag->link), &(task->link));
			if ( tmout != TMO_FEVER ) {
				/* タイムアウトリストに追加 */
				task_add_timeout(task, tmout);
			}
			req_dispatch = schedule(cpu);

			cpu_spinunlock(cpu);
		}
	}

	flag_spinunlock(flag);

	if ( req_dispatch ) {
		self_request_dispatch();
		ret = task->result_code;
	}

	irq_restore(irq_state);
	return ret;
}

int _kernel_flag_wait(FlagStruct* flag, uint32_t pattern, uint32_t wait_mode, uint32_t* ret_pattern)
{
	return _kernel_flag_twait(flag, pattern, wait_mode, ret_pattern, TMO_FEVER);
}

int _kernel_flag_clear(FlagStruct* flag, uint32_t pattern)
{
	uint32_t		irq_state;
	irq_state = flag_spinlock_irq_save(flag);
	flag->value &= ~pattern;
	flag_spinunlock_irq_restore(flag, irq_state);
	return RT_OK;
}

OSAPISTUB int __flag_create(void)
{
	int ret = RT_ERR;
	int flag_id = alloc_flag_id();
	if ( 0 <= flag_id ) {
		FlagStruct* flag = flagid2buffer(flag_id);
		ret = _kernel_flag_create(flag);
		if ( ret == RT_OK ) {
			order_barrier();
			flag->id_initialized = true;
			order_barrier();
			ret = flag_id;
		}
		else {
			free_flag_struct(flag_id);
		}
	}
	return ret;
}

OSAPISTUB int __flag_set(int id, uint32_t pattern)
{
	FlagStruct* flag = flagid2object(id);
	return _kernel_flag_set(flag, pattern);
}

OSAPISTUB int __flag_wait(int id, uint32_t pattern, uint32_t wait_mode, uint32_t* ret_pattern)
{
	FlagStruct* flag = flagid2object(id);
	return _kernel_flag_wait(flag, pattern, wait_mode, ret_pattern);
}

OSAPISTUB int __flag_twait(int id, uint32_t pattern, uint32_t wait_mode, uint32_t* ret_pattern, TimeOut tmout)
{
	FlagStruct* flag = flagid2object(id);
	return _kernel_flag_twait(flag, pattern, wait_mode, ret_pattern, tmout);

}

OSAPISTUB int __flag_clear(int id, uint32_t pattern)
{
	FlagStruct* flag = flagid2object(id);
	return _kernel_flag_clear(flag, pattern);
}

#else
OSAPISTUB int __flag_create(void)
{
	return RT_ERR;
}

OSAPISTUB int __flag_set(int id, uint32_t pattern)
{
	return RT_ERR;
}

OSAPISTUB int __flag_wait(int id, uint32_t pattern, uint32_t wait_mode, uint32_t* ret_pattern)
{
	return RT_ERR;
}

OSAPISTUB int __flag_twait(int id, uint32_t pattern, uint32_t wait_mode, uint32_t* ret_pattern, TimeOut tmout)
{
	return RT_ERR;
}

OSAPISTUB int __flag_clear(int id, uint32_t pattern)
{
	return RT_ERR;
}

#endif
