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
	int	ix;
	for (;;) {
		for (ix=0; ix<100; ix++) {
			ptr[ix] = sys_malloc(128);
			lprintf("malloc1[%d]=%08X\n", ix, ptr[ix]);
			memset(ptr[ix], 0x11, 128);
			task_tsleep(5);
		}
		for (ix=0; ix<100; ix++) {
			if ( ptr[ix] ) {
				lprintf("free1[%d]=%08X\n", ix, ptr[ix]);
				sys_free(ptr[ix]);
			}
			task_tsleep(5);
		}
	}
}

void task2(void)
{
	static void* ptr[100];
		int	ix;
		for (;;) {
			for (ix=0; ix<100; ix++) {
				ptr[ix] = sys_malloc(256);
				lprintf("malloc2[%d]=%08X\n", ix, ptr[ix]);
				memset(ptr[ix], 0x44, 256);
				task_tsleep(8);
			}
			for (ix=0; ix<100; ix++) {
				if ( ptr[ix] ) {
					lprintf("free2[%d]=%08X\n", ix, ptr[ix]);
					sys_free(ptr[ix]);
				}
				task_tsleep(8);
			}
		}
}

void task3(void)
{
static void* ptr[100];
	int	ix;
	for (;;) {
		for (ix=0; ix<100; ix++) {
			ptr[ix] = sys_malloc(512);
			lprintf("malloc3[%d]=%08X\n", ix, ptr[ix]);
			memset(ptr[ix], 0x77, 512);
			task_tsleep(11);
		}
		for (ix=0; ix<100; ix++) {
			if ( ptr[ix] ) {
				lprintf("free3[%d]=%08X\n", ix, ptr[ix]);
				sys_free(ptr[ix]);
			}
			task_tsleep(11);
		}
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
