/*
 * main.c
 *
 *  Created on: 2012/12/01
 *      Author: biwa
 */
#include "common.h"
#include "task.h"
#include "malloc.h"

extern TaskStruct	task_info[];
extern uint32_t		task_info_num;

static uint8_t mem_block[0x100000] __attribute__((aligned(16)));

int main(void)
{
	int ix;
	tprintf("main start\n");
	task_init();
	for (ix=0; ix<task_info_num; ix++) {
		task_create(&task_info[ix]);
	}
	irq_init();
	timer_init();
	lprintf_init();
	sys_malloc_init();
	sys_malloc_add_block(mem_block, sizeof(mem_block));
	schedule();
}
