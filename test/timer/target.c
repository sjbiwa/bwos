/*
 * target.c
 *
 *  Created on: 2014/11/15
 *      Author: biwa
 */
#include "bwos.h"

/* configuration task */
static int		task_struct[16];


typedef	struct {
	TimerInfo	info;
	int			a;
	int			b;
} TestParam;
static void timer_handler(void* param)
{
	TestParam* p = (TestParam*)param;
	lprintf("timer_handler:%d:%d\n", p->a, p->b);
	++(p->b);
}

void task1()
{
	TestParam param[30];
	for ( int ix=0; ix<arrayof(param); ix++ ) {
		int timer = timer_create();
		param[ix].info.cyclic = MSEC(200+ix);
		param[ix].info.tmout = MSEC(100+ix*2);
		param[ix].info.kind = TIMER_CYCLIC;
		param[ix].info.param = (void*)(&param[ix]);
		param[ix].info.handler = timer_handler;
		param[ix].a = timer;
		param[ix].b = ix*2;
		timer_set(timer, &param[ix].info);
		timer_enable(timer, true);
		task_tsleep(MSEC(20));
	}
	task_sleep();
}

TaskCreateInfo	task_info[] = {
		{"TASK1", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task1, 1024, 1024, 6, (void*)512},
};

void main_task(void)
{
	int ix;
	for ( ix=0; ix<arrayof(task_info); ix++ ) {
		task_struct[ix] = task_create(&task_info[ix]);
	}

	task_sleep();
}
