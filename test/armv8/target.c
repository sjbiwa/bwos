#if 1

#include "bwos.h"

/* configuration task */
static int		task_struct[16];

static uint32_t	counter[16];

void task1(uint32_t arg0, uint32_t arg1)
{
	double a = 1.0f;
	for (int i=0;;i++) {
		lprintf("task1:%d:%d\n", arg0, (int)a);
		a += 1.0f;
		task_tsleep(SEC(1));
		counter[arg0]++;
	}
}

void task2(uint32_t arg0, uint32_t arg1)
{
	double a = 1.0f;
	for (int i=0;;i++) {
		lprintf("task2:%d:%d\n", arg0, (int)a);
		a += 2.0f;
		task_tsleep(SEC(1));
		counter[arg0]++;
	}
}

void task3(uint32_t arg0, uint32_t arg1)
{
	double a = 1.0f;
	for (int i=0;;i++) {
		lprintf("task3:%d:%d\n", arg0, (int)a);
		a += 3.0f;
		task_tsleep(SEC(1));
		counter[arg0]++;
	}
}

void task4(uint32_t arg0, uint32_t arg1)
{
	for (int i=0;;i++) {
		lprintf("task4:%d:%d\n", arg0, i);
		task_tsleep(MSEC(8));
		counter[arg0]++;
	}
}

void* get_ptr(void)
{
	volatile uint8_t buff[16];
	lprintf("buff=%08X\n", buff);
	memset(buff, 12, 16);
	return buff;
}

void print_task(uint32_t arg0, uint32_t arg1)
{
	for (;;) {
		lprintf("%d:%d:%d:%d\n", counter[0], counter[1], counter[2], counter[3]);
		task_tsleep(SEC(10));
		get_ptr();
	}
}

TaskCreateInfo	task_info[] = {
		{"TASK1", TASK_ACT|TASK_SYS|TASK_FPU, task1, 0, 256, 0, 5, (void*)0},
		{"TASK2", TASK_ACT|TASK_SYS|TASK_FPU, task2, 0, 256, 0, 6, (void*)1},
		{"TASK3", TASK_ACT|TASK_SYS|TASK_FPU, task3, 0, 256, 0, 5, (void*)2},
//		{"TASK4", TASK_ACT|TASK_SYS, task4, 0, 256, 0, 6, (void*)3},
//		{"PRINTER", TASK_ACT|TASK_SYS, print_task, 0, 256, 0, 6, (void*)0},
};

void main_task(void)
{
	int ix;
	for ( ix=0; ix<arrayof(task_info); ix++ ) {
		task_struct[ix] = task_create(&task_info[ix]);
	}

	task_sleep();
}
#else
/*
 * target.c
 *
 *  Created on: 2014/11/15
 *      Author: biwa
 */
#include "bwos.h"

static int sem1;
static int sem2;

void task2(void)
{
	int ret;
	lprintf("task2\n");
	double a = 1.0f;
	for (;;) {
		lprintf("CPACR:%08X\n", (uint32_t)CPACR_EL1_get());
		ret = sem_trequest(sem1, 3, MSEC(100));
		if ( ret == RT_OK ) {
			lprintf("sem get2_1:%d\n", ret);
		}
		ret = sem_trequest(sem2, 2, MSEC(100));
		if ( ret == RT_OK ) {
			lprintf("sem get2_2:%d\n", ret);
		}
		a += 1.0f;
		tprintf("VAL=%d\n", (int)a);
	}
	task_sleep();
}

void task3(void)
{
	int ret;
	lprintf("task3\n");
	for (;;) {
		ret = sem_trequest(sem1, 2, MSEC(200));
		if ( ret == RT_OK ) {
			lprintf("sem get3_1:%d\n", ret);
		}
		ret = sem_trequest(sem2, 1, MSEC(200));
		if ( ret == RT_OK ) {
			lprintf("sem get3_2:%d\n", ret);
		}
	}
	task_sleep();
}

void task4(void)
{
	lprintf("task4\n");
	task_tsleep(SEC(10));
	for (;;) {
		sem_release(sem1, 2);
		task_tsleep(MSEC(50));
	}
	task_sleep();
}

void task5(void)
{
	lprintf("task5\n");
	task_tsleep(SEC(10));
	for (;;) {
		lprintf("sem rel5\n");
		sem_release(sem2, 2);
		task_tsleep(MSEC(80));
	}
	task_sleep();
}


/* configuration task */
int		task_struct[4];

TaskCreateInfo	task_info[] = {
		{"TASK2", TASK_ACT|TASK_SYS|TASK_FPU, task2, 0, 4096, 0, 6},
		{"TASK3", TASK_ACT, task3, 0, 4096, 0, 5},
		{"TASK4", TASK_ACT, task4, 0, 4096, 0, 1},
		{"TASK5", TASK_ACT, task5, 0, 4096, 0, 5},
};

void main_task(void)
{
	int ix;
	for ( ix=0; ix<arrayof(task_info); ix++ ) {
		task_struct[ix] = task_create(&task_info[ix]);
		lprintf("%d\n", task_struct[ix]);
	}

	/* 共有リソース初期化 */
	sem1 = sem_create(20);
	sem2 = sem_create(15);

	task_sleep();
}

#endif
