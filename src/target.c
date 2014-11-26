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
static void* ptr[100];
	for (;;) {
		for (ix=0; ix<100; ix++) {
		void* ptr = sys_malloc(128);
		printf("")
	}
}

void task2(void)
{
	for (;;) {
	}
}

void task3(void)
{
	for (;;) {
	}
}

static FlagStruct	wait_flag;

void task4(void)
{
	task_sleep();
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
	task_sleep();
	lprintf("start task5\n");

	for (;;) {
		flag_wait(&wait_flag);
		lprintf("wakeup task5\n");
	}
}
