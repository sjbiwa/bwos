/*
 * target.c
 *
 *  Created on: 2014/11/15
 *      Author: biwa
 */
#include "bwos.h"

/* configuration task */
static int		task_struct[16];


void task1(uint32_t arg0, uint32_t arg1)
{
	lprintf("task1\n");
	task_active(task_struct[1], (void*)2048);
	task_active(task_struct[2], (void*)4096);
	for (;;) {
		lprintf("task1\n");
		task_tsleep(SEC(1));
	}
}

void task2(uint32_t arg0, uint32_t arg1)
{
	for (;;) {
		lprintf("task2\n");
		task_tsleep(SEC(2));
	}
}

void task3(uint32_t arg0, uint32_t arg1)
{
	for (;;) {
		lprintf("task3\n");
		task_tsleep(SEC(3));
	}
}

void task4(uint32_t arg0, uint32_t arg1)
{
	for (;;) {
		tprintf("task4\n");
		task_tsleep(SEC(2));
	}
}

TaskCreateInfo	task_info[] = {
		{"TASK1", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task1, 0, 1024, 1024, 5, (void*)128},
		{"TASK2", CPU_CORE0|TASK_FPU|TASK_SYS, task2, 0, 1024, 1024, 6, (void*)256},
		{"TASK3", CPU_CORE0|TASK_FPU|TASK_SYS, task3, 0, 1024, 1024, 6, (void*)512},
		{"TASK4", CPU_CORE1|TASK_ACT|TASK_FPU|TASK_SYS, task4, 0, 1024, 1024, 6, (void*)512},
};

void main_task(void)
{
	int ix;
	for ( ix=0; ix<arrayof(task_info); ix++ ) {
		task_struct[ix] = task_create(&task_info[ix]);
	}

	task_sleep();
}
