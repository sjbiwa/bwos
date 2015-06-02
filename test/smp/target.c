/*
 * target.c
 *
 *  Created on: 2014/11/15
 *      Author: biwa
 */
#include "bwos.h"
#include "cp15reg.h"

typedef	struct {
	uint32_t	buff1[16];
	uint8_t		buff2[8];
} Message;

/* configuration task */
static int		task_struct[32];

static volatile int mutex;
static volatile int fixmb;

void task1(uint32_t arg0, uint32_t arg1)
{
	for (;;) {
		lprintf("CORE=%d:task1\n", CPUID_get());
		task_tsleep(MSEC(100));
	}
}

void task2(uint32_t arg0, uint32_t arg1)
{
	for (;;) {
		lprintf("CORE=%d:task2\n", CPUID_get());
		task_tsleep(MSEC(200));
	}
}

void task3(uint32_t arg0, uint32_t arg1)
{
	Message* ptr;
	for (int ix=0;;ix++ ) {
		lprintf("CORE=%d:task3\n", CPUID_get());
		if ( fixmb_request(fixmb, (void**)(&ptr)) == RT_OK ) {
			lprintf("CORE=%d:%d:get xxxxx ok\n", CPUID_get(), ix);
			memset(ptr, 0x45, sizeof(Message));
			fixmb_release(fixmb, (void*)ptr);
		}
		else {
			lprintf("CORE=%d:%d:get xxxxx error\n", CPUID_get(), ix);
		}
		task_tsleep(MSEC(3));
	}
}

void task4(uint32_t arg0, uint32_t arg1)
{
	Message* ptr;
	for (int ix=0;;ix++ ) {
		lprintf("CORE=%d:task4\n", CPUID_get());
		if ( fixmb_request(fixmb, (void**)(&ptr)) == RT_OK ) {
			lprintf("CORE=%d:%d:get yyyyy ok\n", CPUID_get(), ix);
			memset(ptr, 0x45, sizeof(Message));
			fixmb_release(fixmb, (void*)ptr);
		}
		else {
			lprintf("CORE=%d:%d:get yyyyy error\n", CPUID_get(), ix);
		}
		task_tsleep(MSEC(5));
	}
}

TaskCreateInfo	task_info[] = {
		{"TASK01", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task1, 0, 1024, 1024, 5, (void*)0},
		{"TASK02", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task2, 0, 1024, 1024, 6, (void*)0},
		{"TASK03", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task3, 0, 1024, 1024, 7, (void*)0},
		{"TASK04", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task4, 0, 1024, 1024, 8, (void*)0},
		{"TASK11", CPU_CORE1|TASK_ACT|TASK_FPU|TASK_SYS, task1, 0, 1024, 1024, 5, (void*)1},
		{"TASK12", CPU_CORE1|TASK_ACT|TASK_FPU|TASK_SYS, task2, 0, 1024, 1024, 6, (void*)1},
		{"TASK13", CPU_CORE1|TASK_ACT|TASK_FPU|TASK_SYS, task3, 0, 1024, 1024, 7, (void*)1},
		{"TASK14", CPU_CORE1|TASK_ACT|TASK_FPU|TASK_SYS, task4, 0, 1024, 1024, 8, (void*)1},
		{"TASK01", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task1, 0, 1024, 1024, 5, (void*)0},
		{"TASK02", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task2, 0, 1024, 1024, 6, (void*)0},
		{"TASK03", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task3, 0, 1024, 1024, 7, (void*)0},
		{"TASK04", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task4, 0, 1024, 1024, 8, (void*)0},
		{"TASK11", CPU_CORE1|TASK_ACT|TASK_FPU|TASK_SYS, task1, 0, 1024, 1024, 5, (void*)1},
		{"TASK12", CPU_CORE1|TASK_ACT|TASK_FPU|TASK_SYS, task2, 0, 1024, 1024, 6, (void*)1},
		{"TASK13", CPU_CORE1|TASK_ACT|TASK_FPU|TASK_SYS, task3, 0, 1024, 1024, 7, (void*)1},
		{"TASK14", CPU_CORE1|TASK_ACT|TASK_FPU|TASK_SYS, task4, 0, 1024, 1024, 8, (void*)1},

		{"TASK13", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task3, 0, 1024, 1024, 7, (void*)0},
		{"TASK14", CPU_CORE1|TASK_ACT|TASK_FPU|TASK_SYS, task3, 0, 1024, 1024, 8, (void*)1},
		{"TASK15", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task4, 0, 1024, 1024, 7, (void*)0},
		{"TASK16", CPU_CORE1|TASK_ACT|TASK_FPU|TASK_SYS, task4, 0, 1024, 1024, 8, (void*)1},
};

void main_task(void)
{
	int ix;
	mutex = mutex_create();
	fixmb = fixmb_create(sizeof(Message), 16);
	__dsb();

	for ( ix=0; ix<arrayof(task_info); ix++ ) {
		task_struct[ix] = task_create(&task_info[ix]);
	}

	task_sleep();
}
