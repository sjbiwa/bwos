/*
 * target.c
 *
 *  Created on: 2014/11/15
 *      Author: biwa
 */
#include "api.h"

extern TaskStruct	task_info[];

typedef	struct {
	int	value1;
	int	value2;
	int	value3;
	int	value4;
} TlsValue;

static void delay(void)
{
	volatile int		ix, iy;
	for (ix=0x900000; 0<ix; ix--) {
		iy = ix / 234;
	}
}

#if 0
void task1(void)
{
//	double	a = 1.0;
	for (;;) {
//		TlsValue* tls = task_get_tls(TASK_SELF);
//		tls->value1 += 1;
//		tls->value2 += 2;
//		tls->value3 += 3;
//		tls->value4 += 4;
//		a += 0.1;
//		lprintf("task1::%d.%d\n", (int)a, (int)(a*1000)-(int)a*1000);
		lprintf("task1\n");
		task_tsleep(MSEC(5000));
	}
}

void task2(void)
{
//	double	a = 1.0;
	for (;;) {
//		TlsValue* tls = task_get_tls(&task_info[0]);
//		lprintf("%d %d %d %d\n", tls->value1, tls->value2, tls->value3, tls->value4);
//		a += 0.1;
//		lprintf("task2::%d.%d\n", (int)a, (int)(a*1000)-(int)a*1000);
		lprintf("task2\n");
		task_tsleep(MSEC(500));
	}
}

#else

void task1(void)
{
static void* ptr[100];
	int	ix;
	int count;
	double	a = 1.0;
	for ( count=20; 0 < count; count-- ) {
		for (ix=0; ix<100; ix++) {
			ptr[ix] = sys_malloc_align(128, 8);
			//ptr[ix] = sys_malloc(128);
			lprintf("malloc1(8)[%d]=%08X\n", ix, ptr[ix]);
			if ( ptr[ix] ) {
				memset(ptr[ix], 0x11, 128);
			}
			task_tsleep(MSEC(100));
			a += 0.1;
			lprintf("%d.%d\n", (int)a, (int)(a*1000)-(int)a*1000);
		}
		for (ix=0; ix<100; ix++) {
			if ( ptr[ix] ) {
				lprintf("free1[%d]=%08X\n", ix, ptr[ix]);
				sys_free(ptr[ix]);
			}
			task_tsleep(MSEC(100));
		}
		dump_space();
	}
	task_sleep();
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
			task_tsleep(MSEC(200));
		}
		for (ix=0; ix<100; ix++) {
			if ( ptr[ix] ) {
				lprintf("free2[%d]=%08X\n", ix, ptr[ix]);
				sys_free(ptr[ix]);
			}
			task_tsleep(MSEC(200));
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
			task_tsleep(MSEC(300));
		}
		for (ix=0; ix<100; ix++) {
			if ( ptr[ix] ) {
				lprintf("free3[%d]=%08X\n", ix, ptr[ix]);
				sys_free(ptr[ix]);
			}
			task_tsleep(MSEC(300));
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
		task_tsleep(MSEC(50));
		flag_set(&wait_flag);
		lprintf("set_flag task4\n");
	}
}

void task5(void)
{
	task_sleep();
	lprintf("start task5\n");

	for (;;) {
		flag_wait(&wait_flag, 0);
		lprintf("wakeup task5\n");
	}
}

#endif
