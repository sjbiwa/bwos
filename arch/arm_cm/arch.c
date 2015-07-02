/*
 * arch.c
 *
 *  Created on: 2015/07/01
 *      Author: biwa
 */

#include "kernel.h"
#include "bwos.h"
#include "arm.h"
#include "my_board.h"

extern void	_entry_stub(void);
extern char __heap_start;


/* 最初に1になるビットを左側から探す */
static inline int32_t bit_srch_l(uint32_t val)
{
	int32_t ret = 31;
	while ( (0 <= ret) && ((val & (0x01u<<ret)) == 0) ) {
		ret--;
	}
	return ret;
}

void arch_init_task_create(TaskStruct* task)
{
}

int arch_task_create(TaskStruct* task, void* cre_param)
{
	int ret = RT_OK;
	//arch_init_task(task, cre_param);

	return ret;
}

void arch_task_active(TaskStruct* task, void* act_param)
{
}

void arch_system_preinit(uint32_t cpuid)
{
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

bool arch_can_dispatch(void)
{
	bool ret = true;
	if ( 1/* ハンドラモード ? */ ) {
		ret = false;
		/* PendSVC設定 */
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


void _dispatch()
{
}
