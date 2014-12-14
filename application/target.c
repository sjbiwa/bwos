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

void task0(void)
{
static void* ptr[100];
	int ix;
	for (ix=0;;ix++) {
//		ptr[ix%100] = sys_malloc_align(128, 8);
		ptr[ix%100] = (void*)ix;
		lprintf("message task0:%08X\n", ptr[ix%100]);
		task_tsleep(5);
	}
}

void task1(void)
{
static void* ptr[100];
	int ix;
	for (ix=0;;ix++) {
//		ptr[ix%100] = sys_malloc_align(128, 8);
		ptr[ix%100] = (void*)ix;
		lprintf("message task1:%08X\n", ptr[ix%100]);
		task_tsleep(9);
	}
#if 0
static void* ptr[100];
	int	ix;
	int count;
	for ( count=20; 0 < count; count-- ) {
		for (ix=0; ix<100; ix++) {
			ptr[ix] = sys_malloc_align(128, 8);
			//ptr[ix] = sys_malloc(128);
			lprintf("malloc1(8)[%d]=%08X\n", ix, ptr[ix]);
			if ( ptr[ix] ) {
				memset(ptr[ix], 0x11, 128);
			}
			task_tsleep(1);
		}
		for (ix=0; ix<100; ix++) {
			if ( ptr[ix] ) {
				lprintf("free1[%d]=%08X\n", ix, ptr[ix]);
				sys_free(ptr[ix]);
			}
			task_tsleep(1);
		}
		dump_space();
	}
	task_sleep();
#endif
}

void task2(void)
{
static void* ptr[100];
	int	ix;
	int count;
	for ( count=20; 0 < count; count-- ) {
		for (ix=0; ix<100; ix++) {
			ptr[ix] = sys_malloc_align(256, 16);
			//ptr[ix] = sys_malloc(256);
			lprintf("malloc2(16)[%d]=%08X\n", ix, ptr[ix]);
			if ( ptr[ix] ) {
				memset(ptr[ix], 0x44, 256);
			}
			task_tsleep(2);
		}
		for (ix=0; ix<100; ix++) {
			if ( ptr[ix] ) {
				lprintf("free2[%d]=%08X\n", ix, ptr[ix]);
				sys_free(ptr[ix]);
			}
			task_tsleep(2);
		}
		dump_space();
	}
	task_sleep();
}

void task3(void)
{
static void* ptr[100];
	int	ix;
	int count;
	for ( count=20; 0 < count; count-- ) {
		for (ix=0; ix<100; ix++) {
			ptr[ix] = sys_malloc_align(512, 64);
			//ptr[ix] = sys_malloc(512);
			lprintf("malloc3(64)[%d]=%08X\n", ix, ptr[ix]);
			if ( ptr[ix] ) {
				memset(ptr[ix], 0x77, 512);
			}
			task_tsleep(3);
		}
		for (ix=0; ix<100; ix++) {
			if ( ptr[ix] ) {
				lprintf("free3[%d]=%08X\n", ix, ptr[ix]);
				sys_free(ptr[ix]);
			}
			task_tsleep(3);
		}
		dump_space();
	}
	task_sleep();
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
