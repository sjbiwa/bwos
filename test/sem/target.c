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
	for (;;) {
		ret = sem_trequest(sem1, 3, MSEC(100));
		if ( ret == RT_OK ) {
			lprintf("sem get2_1:%d\n", ret);
		}
		ret = sem_trequest(sem2, 2, MSEC(100));
		if ( ret == RT_OK ) {
			lprintf("sem get2_2:%d\n", ret);
		}
	}
	task_sleep();
}

void task3(void)
{
	int ret;
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
	task_tsleep(SEC(10));
	for (;;) {
		lprintf("sem rel4\n");
		sem_release(sem1, 2);
		task_tsleep(MSEC(50));
	}
	task_sleep();
}

void task5(void)
{
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
		{"TASK2", TASK_ACT|TASK_FPU, task2, 1024, 1024, 6},
		{"TASK3", TASK_ACT|TASK_FPU, task3, 1024, 1024, 5},
		{"TASK4", TASK_ACT|TASK_FPU, task4, 1024, 1024, 5},
		{"TASK5", TASK_ACT|TASK_FPU, task5, 1024, 1024, 5},
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
