/*
 * Copyright (C) 2015 sjbiwa All rights reserved.
 */

/*
 * task.c
 *
 *  Created on: 2012/12/01
 *      Author: biwa
 */

#include "kernel.h"

CpuStruct			cpu_struct[CPU_NUM];

static TaskStruct	init_task_struct;	/* 初期タスク構造体 */


/* オブジェクト<->インデックス変換用 */
OBJECT_INDEX_FUNC(task,TaskStruct,TASK_MAX_NUM);

extern void arch_init_task_create(TaskStruct* task);
extern int arch_task_create(TaskStruct* task, void* cre_param);
extern void arch_task_active(TaskStruct* task, void* act_param);

static void task_sub_init(void)
{
	for ( int cpuid=0; cpuid < CPU_NUM; cpuid++ ) {
		cpu_spininit(&cpu_struct[cpuid]);
		cpu_struct[cpuid].run_queue.pri_bits = 0x00000000;
		for (int ix=0; ix<TASK_PRIORITY_NUM; ix++) {
			link_clear(&cpu_struct[cpuid].run_queue.task[ix]);
		}
		link_clear(&(cpu_struct[cpuid].task_time_out_list));
		cpu_struct[cpuid].ctask = NULL;
		cpu_struct[cpuid].ntask = NULL;
		cpu_struct[cpuid].cpuid = cpuid;
		link_clear(&(cpu_struct[cpuid].timer_list));
	}
}

static void request_dispatch_all(CpuStruct* cpu_struct)
{
	if ( (USE_SMP == 0) || (cpu_struct->cpuid == CPUID_get()) ) {
		/* 起床したタスクが自CPU所属なので dispatch処理をする */
		self_request_dispatch();
	}
	else {
		/* 起床したタスクが他CPU所属なので 割り込み通知する */
		ipi_request_dispatch_one(cpu_struct);
	}
}

static inline int32_t lowest_bit(uint32_t value)
{
	int32_t ret = 0;
	if ( (value & 0x0000ffff) == 0 ) {
		ret = 16;
		value >>= 16;
	}
	if ( (value & 0x00ff) == 0 ) {
		ret += 8;
		value >>= 8;
	}
	if ( (value & 0x0f) == 0 ) {
		ret += 4;
		value >>= 4;
	}
	if ( (value & 0x3) == 0 ) {
		ret += 2;
		value >>= 2;
	}
	if ( (value & 0x1) == 0 ) {
		ret += 1;
	}
	return ret;
}

void task_remove_queue(TaskStruct* task)
{
	RunQueue* run_queue = &(task->cpu_struct->run_queue);
	uint32_t	pri = task->priority;
	link_remove(&(task->link));
	/* 優先度キューに登録がなければ該当ビットをクリア */
	if ( link_is_empty(&run_queue->task[pri]) ) {
		run_queue->pri_bits &= ~(0x00000001u << pri);
	}
}

static void task_add_queue(TaskStruct* task)
{
	RunQueue* run_queue = &(task->cpu_struct->run_queue);
	Link*		top;
	uint32_t	pri = task->priority;
	if ( pri < TASK_PRIORITY_NUM ) {
		top = &run_queue->task[pri];
		link_add_last(top, &(task->link));
		run_queue->pri_bits |= (0x00000001u << pri);
	}
}

bool can_dispatch(void)
{
	return arch_can_dispatch();
}

CpuStruct* get_cpu_struct(void)
{
	return &cpu_struct[CPUID_get()];
}

void _kernel_timer_update(Link* task_time_out_list)
{
#if defined(USE_TICKLESS)
	/* タイマハンドラリストの最初のエントリ取得 */
	TimeSpec next_timeout = _timer_get_next_timeout();
	/* タスクのタイムアウトリストの最初のエントリ取得 */
	Link* link = task_time_out_list->next;
	if ( link != task_time_out_list ) {
		TaskStruct* task = containerof(link, TaskStruct, tlink);
		if ((next_timeout == 0) || ( task->timeout < next_timeout) ) {
			next_timeout = task->timeout;
		}
	}

	/* タイムアウト設定 */
	if ( 0 < next_timeout ) {
		update_first_timeout(next_timeout);
	}
#endif
}

/* タイムアウトキューにタスクを登録する */
static void task_add_timeout_queue(TaskStruct* task)
{
	TaskStruct*	q_task;
	Link*		link;
	/************************************************************/
	/* パターン													*/
	/*  1: タイムアウトキューに登録なし -> キューの先頭に登録	*/
	/*	2: タイムアウトキューの途中								*/
	/*	3: タイムアウトキュー全てより遅い ->　キューの最後尾	*/
	/************************************************************/
	Link* task_time_out_list = &(task->cpu_struct->task_time_out_list);
	link = task_time_out_list->next;
	while ( link != task_time_out_list ) {
		q_task = containerof(link, TaskStruct, tlink);
		/* 対象タスクの起床時間が検索中キューのタスクより早ければ */
		/* その直前に登録する */
		if ( task->timeout <= q_task->timeout ) {
			/* linkの前が登録位置 */
			break;
		}
		link = link->next;
	}
	/* link_add_lastはlinkの直前に入れるので期待通り */
	link_add_last(link, &(task->tlink));

	_kernel_timer_update(&(task->cpu_struct->task_time_out_list));
}

static void task_sleep_stub(TaskStruct* task)
{
	task_remove_queue(task);
	link_clear(&(task->link));
	task_set_wait(task, 0);
}

void task_add_timeout(TaskStruct* task, TimeOut tm)
{
	task->timeout = get_tick_count() + tm;
	task_add_timeout_queue(task);
}

void task_wakeup_stub(TaskStruct* task, int32_t result_code)
{
	if ( task->task_state == TASK_WAIT ) {
		/* 待ちリストに登録されている場合はリストから削除 */
		if ( !link_is_empty(&(task->link)) ) {
			link_remove_clear(&(task->link));
		}

		/* タイムアウトキューに登録されている場合はリストから削除 */
		if ( !link_is_empty(&(task->tlink)) ) {
			link_remove_clear(&(task->tlink));
		}

		task->result_code = result_code;
		task->task_state = TASK_READY;
		task_add_queue(task);
	}
}

void task_tick(void)
{
	Link*			link;
	TaskStruct*		task;
	bool			req_sched = false;
	uint32_t		irq_state;
	TimeSpec		tick_count;
	irq_state = irq_save();
	tick_count = get_tick_count();

	CpuStruct* cpu = &cpu_struct[CPUID_get()];
	cpu_spinlock(cpu);

	Link* task_time_out_list = &(cpu->task_time_out_list);

	for (;;) {
		/* キューは昇順に登録されているので先頭のみチェックしていれば良い */
		/* 先頭タスクが起床されたら次のタスクが先頭タスクになるため */
		link = task_time_out_list->next;
		if ( link == task_time_out_list ) {
			break;
		}
		task = containerof(link, TaskStruct, tlink);
		/* タスクの起床時間が大きかったらそこで終了 */
		if ( tick_count < task->timeout ) {
			break;
		}

		/*---------------------------------------------------------------------------------*/
		/* 休止タスクを起床させるためのコールバック関数呼び出し 						   */
		/*---------------------------------------------------------------------------------*/
		/* wait_func内で wait_object_spinlock -> cpu_spinlock の順序でロック取得を行うため */
		/* ここでcpu_spinunlockする                                                        */
		/*---------------------------------------------------------------------------------*/
		/* task->wait_funcはtask自身の実行中にのみ設定される             				   */
		/* 本関数はtaskが所属するCPUでの割り込みハンドラコンテキストなので、関数実行中は   */
		/* task->wait_funcが変更されることは無い                                           */
		/*---------------------------------------------------------------------------------*/
		if ( task->wait_func != 0 ) {
			cpu_spinunlock(cpu);
			(task->wait_func)(task);
			cpu_spinlock(cpu);
		}
		else {
			/* タスク休止中の場合はwakeup処理 */
			task_wakeup_stub(task, RT_TIMEOUT);
		}

		req_sched = true;
	}
	bool req_dispatch = false;
	if ( req_sched ) {
		req_dispatch = schedule(cpu);
	}
	_kernel_timer_update(task_time_out_list);

	cpu_spinunlock(cpu);

	/* タイマモジュールにタイマexpireを通知 */
	/* タイマハンドラは登録/呼び出しが同一コアでしかできないので */
	/* cpu lockしなくても良い(他コアからアクセスされることが無い) */
	_timer_notify_tick(tick_count);

	if ( req_dispatch ) {
		self_request_dispatch();
	}

	irq_restore(irq_state);
}

/* 指定したCPU用ランキューのスケジュールを行う */
/* 割り込み禁止/対象cpu_spinlock状態で呼び出すこと */
bool schedule(CpuStruct* cpu)
{
	RunQueue* run_queue = &(cpu->run_queue);
	TaskStruct* ntask = cpu->ntask;
	if ( run_queue->pri_bits != 0 ) {
		uint32_t bits = lowest_bit(run_queue->pri_bits);
		cpu->ntask = (TaskStruct*)(run_queue->task[bits].next);
	}
	else {
		cpu->ntask = NULL;
	}
	
	/* 再スケジュール対象CPUが自身であればdispatchが必要であると判断する */
	if ( (cpu == &cpu_struct[CPUID_get()]) || (ntask != cpu->ntask) ) {
		return true;
	}
	return false;
}

uint32_t schedule_any(uint32_t wakeup_cpu_list)
{
	/* 起床/休止した全タスクに対応するcpuについて再スケジュール */
	for ( int cpuid = 0; cpuid < CPU_NUM; cpuid++ ) {
		if ( wakeup_cpu_list & (0x01u << cpuid) ) {
			CpuStruct* cpu = &cpu_struct[cpuid];
			cpu_spinlock(cpu);
			if ( !schedule(cpu) ) {
				/* dispatch不要の場合、該当コアのフラグをクリア */
				wakeup_cpu_list &= ~(0x01u << cpuid);
			}
			cpu_spinunlock(cpu);
		}
	}
	return wakeup_cpu_list;
}

void self_request_dispatch(void)
{
	if ( can_dispatch() ) {
		_dispatch();
	}
	else {
		_delayed_dispatch();
	}
}

static inline void task_init_struct(TaskStruct* task, uint8_t* name, uint32_t task_attr, void* entry,
													uint32_t usr_stack_size, uint32_t priority)
{
	link_clear(&task->link);
	task->save_sp = 0;
	task->arch_tls = 0;
	if ( name ) {
		strncpy(task->name, name, sizeof(task->name)-1);
	}
	else {
		task->name[0] = '\0';
	}
	task->task_attr = task_attr;
	task->entry = entry;
#if !defined(NO_USE_SEPARATE_STACK)
	/* ダミー値の設定(不要？) */
	task->init_sp = 0;
	task->stack_size = 0;
#endif
	task->usr_init_sp = NULL;
	task->usr_stack_size = usr_stack_size;
	task->priority = priority;
	task->tls = 0;
	task->tls_size = 0;
	task->task_state = TASK_STANDBY;
	link_clear(&task->tlink);
	task->timeout = 0;
	task->wait_func = 0;
	task->result_code = 0;
	uint32_t cpuid = CPU_GET(task_attr);
	if ( USE_SMP == 1 ) {
		if ( cpuid == CPU_GET(CPU_SELF) ) {
			cpuid = CPUID_get();
		}
	}
	else {
		cpuid = CPUID_get();
	}
	task->cpu_struct = &cpu_struct[cpuid];
}

void task_init_task_create(void)
{
	/* 初期タスクの生成 */
	task_init_struct(&init_task_struct,
					"INIT_TASK",
					_init_task_create_param.task_attr,
					init_task,
					_init_task_create_param.usr_stack_size,
					1);
	arch_init_task_create(&init_task_struct);
	init_task_struct.task_state = TASK_READY;
	task_add_queue(&init_task_struct);
}

KERNAPI int _kernel_task_create(TaskStruct* task, TaskCreateInfo* info)
{
	task_init_struct(task,
					info->name,
					info->task_attr,
					info->entry,
					info->usr_stack_size,
					info->priority + 1/* priority0は予約のため1増やす*/);

	/* TLS alloc */
	if ( (task->tls == NULL) && (task->tls_size != 0) ) {
		task->tls = __sys_malloc_align(task->tls_size, NORMAL_ALIGN);
		if ( task->tls ) {
			memset(task->tls, 0, task->tls_size);
		}
	}

	/* ARCH depend create (スタックはarch_xxxにて生成) */
	if ( arch_task_create(task, info->cre_param) != RT_OK ) {
		goto ERR_RET;
	}

	if ( task->task_attr & TASK_ACT ) {
		uint32_t		irq_state;
		irq_state = irq_save();
		cpu_spinlock(task->cpu_struct);
		task->task_state = TASK_READY;
		task_add_queue(task);

		bool req_dispatch = schedule(task->cpu_struct);
		cpu_spinunlock(task->cpu_struct);
		if ( req_dispatch ) {
			request_dispatch_all(task->cpu_struct);
		}
		irq_restore(irq_state);
	}
	return RT_OK;

ERR_RET:
	if ( task->tls ) {
		__sys_free(task->tls);
	}
	return RT_ERR;
}

KERNAPI int _kernel_task_active(TaskStruct* task, void* act_param)
{
	uint32_t		irq_state;
	bool req_dispatch = false;

	irq_state = irq_save();
	/* taskはどのコアで動いているかはlockしてみるまで不明なので_by_taskを使う */
	cpu_spinlock_by_task(task);

	if ( task->task_state == TASK_STANDBY ) {
		arch_task_active(task, act_param);
		task->task_state = TASK_READY;
		task_add_queue(task);

		req_dispatch = schedule(task->cpu_struct);
	}
	cpu_spinunlock(task->cpu_struct);

	if ( req_dispatch ) {
		request_dispatch_all(task->cpu_struct);
	}
	irq_restore(irq_state);

	return RT_OK;
}

KERNAPI int _kernel_task_sleep(void)
{
	bool req_dispatch = false;
	uint32_t		irq_state;
	irq_state = irq_save();
	TaskStruct* ctask = CTASK();
	CpuStruct* cpu = ctask->cpu_struct;
	cpu_spinlock(cpu);
	task_sleep_stub(ctask);
	req_dispatch = schedule(cpu);
	cpu_spinunlock(cpu);
	if ( req_dispatch ) {
		self_request_dispatch();
	}
	irq_restore(irq_state);
	return RT_OK;
}

KERNAPI int _kernel_task_wakeup(TaskStruct* task)
{
	bool req_dispatch = false;
	int ret = RT_ERR;
	uint32_t		irq_state;
	irq_state = irq_save();

	/* taskはどのコアで動いているかはlockしてみるまで不明なので_by_taskを使う */
	cpu_spinlock_by_task(task);
	CpuStruct* cpu = task->cpu_struct;

	/* wait状態でリソース待ちが無いときにwakeupする */
	if ( (task->task_state == TASK_WAIT) && link_is_empty(&(task->link)) ) {
		task_wakeup_stub(task, RT_WAKEUP);
		req_dispatch = schedule(cpu);
		ret = RT_OK;
	}

	cpu_spinunlock(cpu);

	if ( req_dispatch ) {
		request_dispatch_all(cpu);
	}

	irq_restore(irq_state);
	return ret;
}

KERNAPI int _kernel_task_tsleep(TimeOut tm)
{
	bool req_dispatch = false;
	uint32_t		irq_state;
	irq_state = irq_save();
	TaskStruct* ctask = CTASK();

	CpuStruct* cpu = ctask->cpu_struct;
	cpu_spinlock(cpu);

	task_sleep_stub(ctask);
	ctask->timeout = get_tick_count() + tm;
	task_add_timeout_queue(ctask);

	req_dispatch = schedule(cpu);

	cpu_spinunlock(cpu);

	if ( req_dispatch ) {
		request_dispatch_all(cpu);
	}

	irq_restore(irq_state);
	return ctask->result_code;
}

KERNAPI int _kernel_task_dormant(void)
{
	bool req_dispatch = false;
	uint32_t		irq_state;
	irq_state = irq_save();
	TaskStruct* ctask = CTASK();
	CpuStruct* cpu = ctask->cpu_struct;
	cpu_spinlock(cpu);

	task_remove_queue(ctask);
	link_clear(&(ctask->link));
	ctask->task_state = TASK_DORMANT;
	req_dispatch = schedule(cpu);
	cpu_spinunlock(cpu);

	if ( req_dispatch ) {
		request_dispatch_all(cpu);
	}

	irq_restore(irq_state);
	for (;;); /* not return */
	return RT_OK;
}

#if USE_SMP == 1
static inline void cpu_ordering(CpuStruct** cpu1, CpuStruct** cpu2)
{
	if ( (*cpu2)->cpuid < (*cpu1)->cpuid ) {
		CpuStruct* temp;
		temp = *cpu1;
		*cpu1 = *cpu2;
		*cpu2 = temp;
	}
}

static void cpu_spinlock_aff(CpuStruct* cpu1, CpuStruct* cpu2)
{
	cpu_ordering(&cpu1, &cpu2);
	cpu_spinlock(cpu1);
	cpu_spinlock(cpu2);
}

static void cpu_spinunlock_aff(CpuStruct* cpu1, CpuStruct* cpu2)
{
	/* 解放側は順序不同で良いはず */
	cpu_spinunlock(cpu1);
	cpu_spinunlock(cpu2);
}

typedef	struct {
	CpuStruct* c_cpu;
	CpuStruct* t_cpu;
} AffinityInfo;

/* affinity切り替え用 */
/* 本関数呼び出し時
 *   - 自taskのコンテキストは保存済
 *   - スタックは独立したスタック(IDLEスタックが○)
 *   - c_cpu/t_cpuはspinlock済
 */
void do_set_affinity(AffinityInfo* info)
{
	CpuStruct* c_cpu = info->c_cpu;
	CpuStruct* t_cpu = info->t_cpu;
	TaskStruct* ctask = c_cpu->ctask;
	TaskStruct* ntask = c_cpu->ntask;

	/* 旧CPUを再スケジュール */
	schedule(c_cpu);

	/* 新CPUを再スケジュール */
	ctask->cpu_struct = t_cpu;
	task_add_queue(ctask);
	schedule(t_cpu);

	cpu_spinunlock_aff(c_cpu, t_cpu);

	/* 新CPUにdispatch要求 */
	ipi_request_dispatch_one(t_cpu);

	/* 自CPU(旧CPU)のdispatch */
	_attach_active_task(c_cpu);
}

/*
 * affinity dispatcherタスクによるCPU affinity拡張実装
 */
typedef	struct {
	TaskStruct*		req_task;
	uint32_t		org_cpu;
	uint32_t		req_cpu;
} CpuAffReq;
;
static struct {
	TaskStruct		aff_dsp_task;	/* cpu affinity dispatcher task */
	MsgqStruct		req_queue;		/* cpu affinity request queue */
} aff_dsp_info[CPU_NUM];

static void aff_dsp_task(void* cre_param, void* sta_param)
{
	int cpuid = CPUID_get();
	for (;;) {
		CpuAffReq areq;
		_kernel_msgq_recv(&aff_dsp_info[cpuid].req_queue, &areq, sizeof(areq));
		lprintf("%d:%d>%d\n", cpuid, areq.org_cpu, areq.req_cpu);
	}
}

TaskStruct* _task_get_ntask(void)
{
	TaskStruct* ntask = NULL;
	CpuStruct* cpu = &cpu_struct[CPUID_get()];
	cpu_spinlock(cpu);
	ntask = cpu->ntask;
	cpu_spinunlock(cpu);
	return ntask;
}

void cpu_affinity_ext_init(void)
{
	TaskCreateInfo aff_dsp_task_info = {
		.name				= "AFF_DSP_TASK",
		.task_attr			= 0,
		.entry				= aff_dsp_task,
		.usr_stack_size		= 4096,
		.tls_size			= 0,
		.priority			= 0,
		.cre_param			= NULL,
	};
	for ( int idx = 0; idx < CPU_NUM; idx++ ) {
		_kernel_msgq_create(&aff_dsp_info[idx].req_queue, sizeof(CpuAffReq) * 8);
		aff_dsp_task_info.task_attr = CPU_DEF(idx) | TASK_ACT | TASK_SYS;
		_kernel_task_create(&aff_dsp_info[idx].aff_dsp_task, &aff_dsp_task_info);
	}
}

KERNAPI int _kernel_task_set_affinity(uint32_t aff)
{
	int ret = RT_ERR;
	uint32_t		irq_state;
	irq_state = irq_save();
	TaskStruct* ctask = CTASK();
	if ( (aff < CPU_NUM) && (ctask->cpu_struct->cpuid != aff) ) {
		if ( true ) {
			int cpuid = CPUID_get();
			CpuAffReq areq = {
				.req_task = ctask,
				.org_cpu = cpuid,
				.req_cpu = aff,
			};
			_kernel_msgq_send(&aff_dsp_info[cpuid].req_queue, &areq, sizeof(areq));
			if ( false ) {
				if ( aff_dsp_info[cpuid].req_queue.data_num != 0 ) {
					for (;;) {
						tprintf("Error.....\n");
					}
				}
				/* この後はaff_taskにdispatchしてCPUが切り替わる */
			}
		}
		if ( true ) {
			CpuStruct* c_cpu = ctask->cpu_struct;
			CpuStruct* t_cpu = &cpu_struct[aff];
	
			cpu_spinlock_aff(c_cpu, t_cpu);
	
			/* taskをrun_queueから切り離し */
			task_remove_queue(ctask);
			link_clear(&(ctask->link));
	
			AffinityInfo aff_info;
			aff_info.c_cpu = c_cpu;
			aff_info.t_cpu = t_cpu;
			_detach_active_task(c_cpu, &aff_info);
		}
		ret = RT_OK;
	}

	irq_restore(irq_state);
	return ret;
}
#endif

KERNAPI int _kernel_task_get_tls(TaskStruct* task, void** ptr)
{
	if ( task == TASK_SELF ) {
		task = CTASK();
	}
	*ptr = (void*)(task->tls);
	return RT_OK;
}

OSAPISTUB int __task_create(TaskCreateInfo* info)
{
	int ret = RT_ERR;
	int task_id = alloc_task_id();
	if ( 0 <= task_id ) {
		TaskStruct* task = taskid2buffer(task_id);
		ret = _kernel_task_create(task, info);
		if ( ret == RT_OK ) {
			order_barrier();
			task->id_initialized = true;
			order_barrier();
			ret = task_id;
		}
		else {
			free_task_struct(task_id);
		}
	}
	return ret;
}

OSAPISTUB int __task_active(int id, void* act_param)
{
	int ret = RT_ERR;
	TaskStruct* task = taskid2object(id);
	if ( task ) {
		ret = _kernel_task_active(task, act_param);
	}
	return ret;
}

OSAPISTUB int __task_sleep(void)
{
	return _kernel_task_sleep();
}

OSAPISTUB int __task_wakeup(int id)
{
	int ret = RT_ERR;
	TaskStruct* task = taskid2object(id);
	if ( task ) {
		ret = _kernel_task_wakeup(task);
	}
	return ret;
}

OSAPISTUB int __task_tsleep(TimeOut tm)
{
	return _kernel_task_tsleep(tm);
}

OSAPISTUB int __task_dormant(void)
{
	return _kernel_task_dormant();
}

OSAPISTUB int __task_get_tls(int id, void** ptr)
{
	int ret = RT_ERR;
	TaskStruct* task = taskid2object(id);
	if ( task ) {
		ret = _kernel_task_get_tls(task, ptr);
	}
	return ret;
}

#if USE_SMP == 1
OSAPISTUB int __task_set_affinity(uint32_t aff)
{
	return _kernel_task_set_affinity(aff);
}
#endif
