/*
 * target.c
 *
 *  Created on: 2014/11/15
 *      Author: biwa
 */
#include "api.h"

static volatile int flag = 0;

static double calc_value(double a, double b, double c)
{
	return a+b+c+1.2;
}

void task1(void)
{
	int ix;
	double a = 2.0, b = 3.0, c = 4.0;

	for (ix=0; ix < 10000; ix++) {
		a += 1.0;
		b += 2.0;
		c += 3.0;
		task_tsleep(MSEC(1));
	}
		lprintf("%d %d %d\n", (int)a, (int)b, (int)c);
	flag = 1;
	task_sleep();
}

void task2(void)
{
	int ix;
	double a = 2.0, b = 3.0, c = 4.0;

	for (ix=0; ix < 10000; ix++) {
		a += 1.0;
		b += 2.0;
		c += 3.0;
		task_tsleep(MSEC(2));
	}
		lprintf("%d %d %d\n", (int)a, (int)b, (int)c);
	flag = 1;
	task_sleep();
}

void task3(void)
{
	int ix;
	double a = 2.0, b = 3.0, c = 4.0;

	for (ix=0; ix < 10000; ix++) {
		a += 1.0;
		b += 2.0;
		c += 3.0;
		task_tsleep(MSEC(3));
	}
	lprintf("%d %d %d\n", (int)a, (int)b, (int)c);
	flag = 1;
	task_sleep();
}

static FlagStruct	wait_flag;

void task4(void)
{
	flag_create(&wait_flag);
	lprintf("start task4\n");
	while ( flag == 0 ) {
		task_tsleep(MSEC(5));
		flag_set(&wait_flag);
		lprintf("set_flag task4\n");
	}
	task_sleep();
}

void task5(void)
{
	lprintf("start task5\n");

	for (;;) {
		flag_wait(&wait_flag, 0);
		lprintf("wakeup task5\n");
	}
}
