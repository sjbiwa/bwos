/*
 * target.c
 *
 *  Created on: 2014/11/15
 *      Author: biwa
 */
#include "bwos.h"

/* configuration task */
static int		task_struct[32];

static volatile int mutex;

void task1(uint32_t arg0, uint32_t arg1)
{
	for (;;) {
		lprintf("CORE=%d:task1\n", arg0);
		task_tsleep(MSEC(100));
	}
}

void task2(uint32_t arg0, uint32_t arg1)
{
	for (;;) {
		lprintf("CORE=%d:task2\n", arg0);
		task_tsleep(MSEC(200));
	}
}

void task3(uint32_t arg0, uint32_t arg1)
{
	for (;;) {
		lprintf("CORE=%d:task3\n", arg0);
		task_tsleep(MSEC(300));
	}
}

void task4(uint32_t arg0, uint32_t arg1)
{
	for (;;) {
		lprintf("CORE=%d:task4\n", arg0);
		task_tsleep(SEC(1));
	}
}

TaskCreateInfo	task_info[] = {
		{"TASK01", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task1, 0, 1024, 1024, 5, (void*)0},
		{"TASK02", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task2, 0, 1024, 1024, 6, (void*)0},
		{"TASK03", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task3, 0, 1024, 1024, 7, (void*)0},
		{"TASK04", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task4, 0, 1024, 1024, 8, (void*)0},
		{"TASK11", CPU_CORE1|TASK_ACT|TASK_FPU|TASK_SYS, task1, 0, 1024, 1024, 5, (void*)1},
		{"TASK12", CPU_CORE1|TASK_ACT|TASK_FPU|TASK_SYS, task2, 0, 1024, 1024, 6, (void*)1},
		{"TASK13", CPU_CORE1|TASK_ACT|TASK_FPU|TASK_SYS, task3, 0, 1024, 1024, 7, (void*)1},
		{"TASK14", CPU_CORE1|TASK_ACT|TASK_FPU|TASK_SYS, task4, 0, 1024, 1024, 8, (void*)1},
		{"TASK01", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task1, 0, 1024, 1024, 5, (void*)0},
		{"TASK02", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task2, 0, 1024, 1024, 6, (void*)0},
		{"TASK03", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task3, 0, 1024, 1024, 7, (void*)0},
		{"TASK04", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task4, 0, 1024, 1024, 8, (void*)0},
		{"TASK11", CPU_CORE1|TASK_ACT|TASK_FPU|TASK_SYS, task1, 0, 1024, 1024, 5, (void*)1},
		{"TASK12", CPU_CORE1|TASK_ACT|TASK_FPU|TASK_SYS, task2, 0, 1024, 1024, 6, (void*)1},
		{"TASK13", CPU_CORE1|TASK_ACT|TASK_FPU|TASK_SYS, task3, 0, 1024, 1024, 7, (void*)1},
		{"TASK14", CPU_CORE1|TASK_ACT|TASK_FPU|TASK_SYS, task4, 0, 1024, 1024, 8, (void*)1},
};

void main_task(void)
{
	int ix;
	mutex = mutex_create();
	__dsb();

	for ( ix=0; ix<arrayof(task_info); ix++ ) {
		task_struct[ix] = task_create(&task_info[ix]);
	}

	task_sleep();
}
