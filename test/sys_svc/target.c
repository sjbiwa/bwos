/*
 * target.c
 *
 *  Created on: 2014/11/15
 *      Author: biwa
 */
#include "api.h"

FlagStruct flag;

void task1(void)
{
	lprintf("TASK1\n");
	for (;;) {
		int ret_pat;
		int ret = flag_twait(&flag, 0x0003, FLAG_OR|FLAG_CLR, &ret_pat, SEC(4));
		lprintf("task1:flag:%d\n", ret);
	}
}

void task2(void)
{
	lprintf("TASK2\n");
	for (;;) {
		task_tsleep(SEC(5));
		irq_disable();
		irq_enable();
	}
}

void task3(void)
{
	lprintf("TASK3\n");
	for (;;) {
		task_tsleep(MSEC(7500));
		flag_set(&flag, 0x0001);
		irq_disable();
		irq_enable();
	}
}

/* configuration task */
#define	MAX_TASK				(3)
TaskStruct		task_struct[MAX_TASK];

TaskCreateInfo	task_info[MAX_TASK] = {
		{"TASK1", TASK_ACT|TASK_FPU, task1, 0, 1024, 1024, 5},
		{"TASK2", TASK_ACT|TASK_FPU, task2, 0, 1024, 1024, 6},
		{"TASK3", TASK_ACT|TASK_FPU|TASK_SYS, task3, 0, 1024, 1024, 7},
};

void init_task(void)
{
	int ix;
	for ( ix=0; ix<arrayof(task_info); ix++ ) {
		task_create(&task_struct[ix], &task_info[ix]);
	}

	flag_create(&flag);
	task_sleep();
}
