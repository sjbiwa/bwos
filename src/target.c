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
		int32_t ret = task_tsleep(200);
		irq_disable();
		tprintf("task1 : wakeup=%d\n", ret);
		irq_enable();
	}
}

void task2(void)
{
	for (;;) {
		task_tsleep(700);
		task_wakeup(&task_info[0]);
		irq_disable();
		tprintf("task2 : wakeup\n");
		irq_enable();
	}
}

void task3(void)
{
	for (;;) {
		irq_disable();
		tprintf("task3 : time-out\n");
		irq_enable();
		task_tsleep(500);
	}
}

static FlagStruct	wait_flag;

void task4(void)
{
	flag_create(&wait_flag);
	irq_disable();
	tprintf("start task4\n");
	irq_enable();
	for (;;) {
		task_tsleep(500);
		flag_set(&wait_flag);
	}
}

void task5(void)
{
	irq_disable();
	tprintf("start task5\n");
	irq_enable();

	for (;;) {
		flag_wait(&wait_flag);
		irq_disable();
		tprintf("wakeup task5\n");
		irq_enable();
	}
}
