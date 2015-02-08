/*
 * target.c
 *
 *  Created on: 2014/11/15
 *      Author: biwa
 */
#include "api.h"

static void task1(void)
{
static void* ptr[100];
	int	ix;
	int count;
	double	a = 1.0;
	for ( count=20; 0 < count; count-- ) {
		for (ix=0; ix<100; ix++) {
			ptr[ix] = sys_malloc_align(128, 8);
			lprintf("malloc1(8)[%d]=%08X\n", ix, ptr[ix]);
			if ( ptr[ix] ) {
				memset(ptr[ix], 0x11, 128);
			}
			task_tsleep(MSEC(10));
			a += 0.1;
			lprintf("%d.%d\n", (int)a, (int)(a*1000)-(int)a*1000);
		}
		for (ix=0; ix<100; ix++) {
			if ( ptr[ix] ) {
				lprintf("free1[%d]=%08X\n", ix, ptr[ix]);
				sys_free(ptr[ix]);
			}
			task_tsleep(MSEC(10));
		}
	}
	task_sleep();
}

static void task2(void)
{
static void* ptr[100];
	int	ix;
	int count;
	for ( count=20; 0 < count; count-- ) {
		for (ix=0; ix<100; ix++) {
			ptr[ix] = sys_malloc_align(256, 16);
			lprintf("malloc2(16)[%d]=%08X\n", ix, ptr[ix]);
			if ( ptr[ix] ) {
				memset(ptr[ix], 0x44, 256);
			}
			task_tsleep(MSEC(20));
		}
		for (ix=0; ix<100; ix++) {
			if ( ptr[ix] ) {
				lprintf("free2[%d]=%08X\n", ix, ptr[ix]);
				sys_free(ptr[ix]);
			}
			task_tsleep(MSEC(20));
		}
	}
	task_sleep();
}

static volatile bool flags = false;
static void task3(void)
{
static void* ptr[100];
	int	ix;
	int count;
	for ( count=20; 0 < count; count-- ) {
		for (ix=0; ix<100; ix++) {
			ptr[ix] = sys_malloc_align(512, 64);
			lprintf("malloc3(64)[%d]=%08X\n", ix, ptr[ix]);
			if ( ptr[ix] ) {
				memset(ptr[ix], 0x77, 512);
			}
			task_tsleep(MSEC(30));
		}
		for (ix=0; ix<100; ix++) {
			if ( ptr[ix] ) {
				lprintf("free3[%d]=%08X\n", ix, ptr[ix]);
				sys_free(ptr[ix]);
			}
			task_tsleep(MSEC(30));
		}
	}
	flags = true;
	task_sleep();
}

static FlagStruct*	wait_flag;

static void task4(void)
{
	flag_create(&wait_flag);
	lprintf("start task4\n");
	task_tsleep(MSEC(70));
	flag_set(wait_flag, 0x0001);
	lprintf("set_flag task4\n");
	task_sleep();
}

static void task5(void)
{
	lprintf("start task5\n");

	while (!flags) {
		uint32_t ret_pattern;
		int ret = flag_twait(wait_flag, 0x0001, FLAG_OR|FLAG_CLR, &ret_pattern, MSEC(50));
		lprintf("wakeup task5:%d\n", ret);
	}
	task_sleep();
}

/* configuration task */
static TaskStruct*		task_struct[16];

static TaskCreateInfo	task_info[] = {
		{"TASK1", TASK_ACT|TASK_FPU, task1, 0, 1024, 1024, 5},
		{"TASK2", TASK_ACT|TASK_FPU, task2, 0, 1024, 1024, 5},
		{"TASK3", TASK_ACT|TASK_FPU, task3, 0, 1024, 1024, 5},
		{"TASK4", TASK_ACT, task4, 0, 1024, 1024, 5},
		{"TASK5", TASK_ACT, task5, 0, 1024, 1024, 5},
};

void main3_task(void)
{
	int ix;
	for ( ix=0; ix<arrayof(task_info); ix++ ) {
		task_create(&task_struct[ix], &task_info[ix]);
	}
}
