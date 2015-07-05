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
	for (;;) {
		lprintf("task1\n");
		task_tsleep(SEC(2));
	}
}

void task2(uint32_t arg0, uint32_t arg1)
{
	for (;;) {
		lprintf("task2\n");
		task_tsleep(SEC(3));
	}
}

TaskCreateInfo	task_info[] = {
		{"TASK1", TASK_ACT|TASK_FPU|TASK_SYS, task1, 0, 1024, 1024, 5, (void*)128},
		{"TASK2", TASK_ACT|TASK_FPU|TASK_SYS, task2, 0, 1024, 1024, 6, (void*)128},
};

void main_task(void)
{
	int ix;
	for ( ix=0; ix<arrayof(task_info); ix++ ) {
		task_struct[ix] = task_create(&task_info[ix]);
	}

	task_sleep();
}
