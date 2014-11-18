/*
 * target.c
 *
 *  Created on: 2014/11/15
 *      Author: biwa
 */
#include "common.h"
#include "task.h"
#include "flag.h"

extern TaskStruct	task_info[];

static void delay(void)
{
	volatile int		ix, iy;
	for (ix=0x900000; 0<ix; ix--) {
		iy = ix / 234;
	}
}

void task1(void)
{
	for (;;) {
		int32_t ret = task_tsleep(1);
		lprintf("task1 : wakeup=%d\n", ret);
	}
}

void task2(void)
{
	for (;;) {
		task_tsleep(1);
		task_wakeup(&task_info[0]);
		lprintf("task2 : wakeup\n");
	}
}

void task3(void)
{
	for (;;) {
		lprintf("task3 : time-out\n");
		task_tsleep(1);
	}
}

static FlagStruct	wait_flag;

void task4(void)
{
	flag_create(&wait_flag);
	lprintf("start task4\n");
	for (;;) {
		task_tsleep(50);
		flag_set(&wait_flag);
		lprintf("set_flag task4\n");
	}
}

void task5(void)
{
	lprintf("start task5\n");

	for (;;) {
		flag_wait(&wait_flag);
		lprintf("wakeup task5\n");
	}
}
