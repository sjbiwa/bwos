/*
 * target.c
 *
 *  Created on: 2014/11/15
 *      Author: biwa
 */
#include "bwos.h"

typedef	struct {
	uint32_t id;
	uint32_t count;
} Message;

static int	fixmb;
static int	msgq;

void task1(void)
{
	Message* ptr;
	uint32_t ix;
	for ( ix=0;; ix++ ) {
		if ( fixmb_request(fixmb, (void**)(&ptr)) == RT_OK ) {
			lprintf("TASK1:FIXMB:%08X\n", ptr);
			ptr->id = 1;
			ptr->count = ix;
			msgq_send(msgq, &ptr, sizeof(void*));
			task_tsleep(MSEC(15));
		}
	}
}

void task2(void)
{
	Message* ptr;
	uint32_t ix;
	for ( ix=0;; ix++ ) {
		if ( fixmb_request(fixmb, (void**)(&ptr)) == RT_OK ) {
			lprintf("TASK2:FIXMB:%08X\n", ptr);
			ptr->id = 2;
			ptr->count = ix*10000;
			msgq_send(msgq, &ptr, sizeof(void*));
			task_tsleep(MSEC(50));
		}
	}
}

void task3(void)
{
	Message* ptr;
	for (;;) {
		if ( msgq_recv(msgq, &ptr, sizeof(void*)) == RT_OK ) {
			lprintf("TASK3:RECV(%d)/(%d):\n", ptr->id, ptr->count);
			fixmb_release(fixmb, (void*)ptr);
		}
		else {
			lprintf("TASK3:RECV:ERROR:\n");
		}
	}
	task_sleep();
}

/* configuration task */
int		task_struct[16];

TaskCreateInfo	task_info[] = {
		{"TASK1", TASK_ACT|TASK_FPU, task1, 0, 1024, 1024, 5},
		{"TASK2", TASK_ACT|TASK_FPU, task2, 0, 1024, 1024, 6},
		{"TASK3", TASK_ACT|TASK_FPU, task3, 0, 1024, 1024, 7},
};

void main_task(void)
{
	int ix;
	for ( ix=0; ix<arrayof(task_info); ix++ ) {
		task_struct[ix] = task_create(&task_info[ix]);
	}

	/* 共有リソース初期化 */
	fixmb = fixmb_create(128, 16);
	msgq = msgq_create(1024);
	task_sleep();
}
