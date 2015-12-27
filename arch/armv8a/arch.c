/*
 * Copyright (C) 2015 sjbiwa All rights reserved.
 */

/*
 * arch.c
 *
 *  Created on: 2015/12/27
 *      Author: biwa
 */

#include "kernel.h"
#include "bwos.h"
#include "armv8.h"
#include "my_board.h"
#include "armv8reg.h"
#include "gicv3reg.h"

extern void	_entry_stub(void);
extern char __heap_start;

CpuStruct* cpu_struct_pointer[CPU_NUM]; /* 各コアが自身のcpu_structを取り出すためのもの */


/* 最初に1になるビットを左側から探す */
static inline int32_t bit_srch_l(uint32_t val)
{
	int32_t ret = 31;
	while ( (0 <= ret) && ((val & (0x01u<<ret)) == 0) ) {
		ret--;
	}
	return ret;
}

static inline void arch_init_task(TaskStruct* task, void* cre_param)
{
	uint64_t*		ptr;
	uint64_t*		usr_stack;

	/* SVC stack */
	ptr = (uint64_t*)((PtrInt_t)(task->init_sp) + task->stack_size - TASK_FRAME_SIZE);
	ptr = PRE_ALIGN_BY(ptr, STACK_ALIGN);
	task->save_sp = ptr;

	if ( !(task->task_attr & TASK_SYS) ) {
		/* USR stack */
		usr_stack = (uint64_t*)((PtrInt_t)(task->usr_init_sp) + task->usr_stack_size);
		usr_stack = PRE_ALIGN_BY(usr_stack, STACK_ALIGN);
	}
	else {
		usr_stack = 0;
	}

	/* setup task-context */
	ptr[TASK_FRAME_STUB] = (PtrInt_t)_entry_stub;
	ptr[TASK_FRAME_PC] = (PtrInt_t)task->entry;
	ptr[TASK_FRAME_PSR] = (task->task_attr&TASK_SYS)?MODE_SYS:MODE_USR;
	ptr[TASK_FRAME_CPACR] = 0x00000000;
	ptr[TASK_FRAME_SP_EL0] = (PtrInt_t)usr_stack;
	ptr[TASK_FRAME_LR] = (PtrInt_t)task_dormant;
	/* Task entry arg */
	ptr[TASK_FRAME_X0] = (uint32_t)cre_param;
	ptr[TASK_FRAME_X1] = 0;
	ptr[TASK_FRAME_X2] = 0;
	ptr[TASK_FRAME_X3] = 0;
}

void arch_init_task_create(TaskStruct* task)
{
	task->stack_size = TASK_SVC_STACK_SIZE;
	if ( task->task_attr & TASK_SYS ) {
		/* SYSタスク */
		/* タスク用スタックとSVC用スタックは共用 */
		task->stack_size += task->usr_stack_size;
		task->usr_init_sp = 0;
	}
	else {
		/* USRタスク */
		/* タスク用スタックとSVC用スタックは個別 */
		task->usr_init_sp = sys_malloc_align_body(task->usr_stack_size, STACK_ALIGN);
	}
	task->init_sp = sys_malloc_align_body(task->stack_size, STACK_ALIGN);

	arch_init_task(task, NULL);
}

int arch_task_create(TaskStruct* task, void* cre_param)
{
	int ret = RT_OK;

	/* スタック領域の確保 */
	if ( task->task_attr & TASK_SYS ) {
		/* SYSタスク */
		task->stack_size = task->usr_stack_size + TASK_SVC_STACK_SIZE;
		task->usr_init_sp = 0;
	}
	else {
		/* USRタスク */
		/* USRスタック確保 */
		task->usr_init_sp = __sys_malloc_align(task->usr_stack_size, STACK_ALIGN);
		if ( !(task->usr_init_sp) ) {
			goto ERR_RET;
		}
	}

	/* SYSスタック確保 */
	task->init_sp = __sys_malloc_align(task->stack_size, STACK_ALIGN);
	if ( !(task->init_sp) ) {
		goto ERR_RET;
	}

	arch_init_task(task, cre_param);

	/* FPU(VFP)退避用領域の確保 */
	if ( task->task_attr & TASK_FPU ) {
		uint64_t*		ptr = task->save_sp;
		ptr[TASK_FRAME_CPACR] = 0x00300000;
		task->arch_tls = __sys_malloc_align((32*2+2)*8, STACK_ALIGN); /* V0-V31(128bit), FPCR(64bit), FPSR(64bit) */
		if ( !(task->arch_tls) ) {
			ret = RT_ERR;
		}
	}
	return ret;

ERR_RET:
	if ( task->init_sp ) {
		__sys_free(task->init_sp);
	}
	if ( task->usr_init_sp ) {
		__sys_free(task->usr_init_sp);
	}
	return RT_ERR;
}

void arch_task_active(TaskStruct* task, void* act_param)
{
	uint64_t*		ptr = task->save_sp;
	ptr[TASK_FRAME_X1] = (uint64_t)act_param;
}

static void
smp0_handler(uint32_t irqno, void* info)
{
}

void arch_system_preinit(uint32_t cpuid)
{
	if ( cpuid == MASTER_CPU_ID ) {
		for (int cpuid=0; cpuid < CPU_NUM; cpuid++ ) {
			cpu_struct_pointer[cpuid] = &cpu_struct[cpuid];
		}
	}

	/* 割り込みコントローラ初期化 */
	arch_irq_init(cpuid);
}

void arch_register_st_memory()
{
	/* 起動時メモリ登録 */
	st_malloc_init(&__heap_start, PTRVAR(END_MEM_ADDR+1) - PTRVAR(&__heap_start));
}
arch_register_normal_memory(void)
{
}

void arch_system_postinit(uint32_t cpuid)
{
}

void _delayed_dispatch(void)
{
}

bool arch_can_dispatch(void)
{
extern	uint32_t _irq_level[CPU_NUM]; /* 多重割り込みレベル */
	bool ret = false;
	if ( _irq_level[CPUID_get()] == 0 ) {
		ret = true;
	}
	return ret;
}

void ipi_request_dispatch(uint32_t other_cpu_list)
{
}

void ipi_request_dispatch_one(CpuStruct* cpu)
{
}

void init_task_arch_depend(void)
{
extern void init_task_board_depend(void);
	init_task_board_depend();
}

/* ０除算呼び出し */
void raise(void)
{
	irq_disable();
	tprintf("0 divided\n");
	/* ０番地にアクセスして例外を発生させる */
	uint32_t value = *((volatile uint32_t*)0);
	for (;;);
}
