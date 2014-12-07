/*
 * arch.c
 *
 *  Created on: 2014/12/07
 *      Author: biwa
 */

#include "common.h"
#include "task.h"
#include "arm.h"

void arch_task_create(TaskStruct* task)
{
	extern void	_entry_stub(void);
	uint32_t*		ptr;

	/* setup stack pointer */
	ptr = (uint32_t*)((uint32_t)(task->init_sp) + task->stack_size - TASK_FRAME_SIZE);
	task->save_sp = (void*)ptr;
	/* setup task-context */
	ptr[TASK_FRAME_STUB] = (void*)_entry_stub;
	ptr[TASK_FRAME_PC] = (uint32_t)task->start_entry;
	ptr[TASK_FRAME_PSR] = (cpsr_get() | FLAG_T ) & ~FLAG_I;
}

void arch_system_preinit(void)
{
}

void arch_system_postinit(void)
{
	extern char __heap_start;
	uint32_t size = (uint8_t*)0x01000000 - (uint8_t*)(&__heap_start);
	size &= ~0x0000000fu;
	tprintf("mblock addr=%08X size=%08X\n", &__heap_start, size);
	sys_malloc_add_block(&__heap_start, size);
}
