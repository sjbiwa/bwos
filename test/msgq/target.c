/*
 * target.c
 *
 *  Created on: 2014/11/15
 *      Author: biwa
 */
#include "api.h"

static MsgqStruct	msgq1;
static MsgqStruct	msgq2;

void task1(void)
{
	int ix;
	void* ptr;
	for ( ix=0; ix < 100000; ix++ ) {
		while ( (ptr = sys_malloc(1000)) == 0 ) {
			task_tsleep(MSEC(100));
		}
		msgq_send(&msgq1, ptr);
		lprintf("TASK1:MSGQ1:send:%d\n", ix);
	}
	task_sleep();
}

void task2(void)
{
	int ix;
	void* ptr;
	for ( ix=0;; ix++ ) {
		msgq_recv(&msgq1, &ptr);
		lprintf("TASK2:MSG1:recv:%d\n", ix);
		msgq_send(&msgq2, ptr);
	}
}

void task3(void)
{
	int ix;
	void* ptr;
	for ( ix=0;; ix++ ) {
		msgq_recv(&msgq2, &ptr);
		sys_free(ptr);
		lprintf("TASK3:MSG2:recv:%d\n", ix);
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

	/* 共有リソース初期化 */
	msgq_create(&msgq1, 16);
	msgq_create(&msgq2, 16);

	task_sleep();
}
