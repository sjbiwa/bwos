/*
 * target.c
 *
 *  Created on: 2014/11/15
 *      Author: biwa
 */
#include "api.h"

static FixmbStruct	fixmb;

static volatile void* alloc_ptr[32];

void task1(void)
{
	int ix;
	void* ptr;
	for ( ix=0; ix<32; ix++ ) {
		task_tsleep(SEC(1));
		if ( fixmb_request(&fixmb, &ptr) == RT_OK ) {
			alloc_ptr[ix] = ptr;
			lprintf("TASK1:FIXMB:%08X\n", ptr);
		}
		else {
			lprintf("TASK1:FIXMB:ERROR\n");
		}

	}
	task_sleep();
}

void task2(void)
{
	int ix;
	task_tsleep(SEC(20));
	for ( ix=0; ix<32; ix++ ) {
		while ( !alloc_ptr[ix] ) {
			task_tsleep(SEC(1));
		}
		if ( fixmb_release(&fixmb, alloc_ptr[ix]) == RT_OK ) {
			lprintf("TASK2:FIXMB:%08X\n", alloc_ptr[ix]);
			alloc_ptr[ix] = 0;
		}
		else {
			lprintf("TASK2:FIXMB:ERROR\n");
		}

	}
	task_sleep();
}

void task3(void)
{
	task_sleep();
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

	/* 共有リソース初期化 */
	fixmb_create(&fixmb, 128, 16);

	task_sleep();
}
