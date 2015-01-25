/*
 * target.c
 *
 *  Created on: 2014/11/15
 *      Author: biwa
 */
#include "api.h"

void task1(void)
{
	lprintf("TASK1\n");
	for (;;) {
		lprintf("--1111111111111111111111111111111111111111111111111111111---\n");
		task_tsleep(SEC(1));
	}
}

void task2(void)
{
	lprintf("TASK2\n");
	for (;;) {
		lprintf("--XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX---\n");
		task_tsleep(MSEC(500));
	}
}

void task3(void)
{
	lprintf("TASK3\n");
	for (;;) {
		lprintf("------------------------------------------------------------\n");
	}
}

/* configuration task */
TaskStruct		task_struct[16];

TaskCreateInfo	task_info[] = {
		{"TASK1", TASK_ACT|TASK_FPU, task1, 0, 1024, 1024, 5},
		{"TASK2", TASK_ACT|TASK_FPU, task2, 0, 1024, 1024, 6},
		{"TASK3", TASK_ACT|TASK_FPU, task3, 0, 1024, 1024, 7},
};

void init_task(void)
{
	int ix;
	for ( ix=0; ix<arrayof(task_info); ix++ ) {
		task_create(&task_struct[ix], &task_info[ix]);
	}

	task_sleep();
}
