/*
 * target.c
 *
 *  Created on: 2014/11/15
 *      Author: biwa
 */
#include "bwos.h"

/* configuration task */
static int		task_struct[16];

static void led_flash(void)
{
static uint32_t delay = 0;
static bool		up = false;
	if ( delay == 0) {
		if ( up ) {
			Board_LED_Set(0, 0);
			up = false;
		}
		else {
			Board_LED_Set(0, 1);
			up = true;
		}
		delay = 1000;
	}
	else {
		delay--;
	}
}

void task1(uint32_t arg0, uint32_t arg1)
{
	lprintf("task1\n");
	for (;;) {
		Board_LED_Set(0, 0);
		task_tsleep(MSEC(200));
		Board_LED_Set(0, 1);
		task_tsleep(MSEC(100));
	}
}

void task2(uint32_t arg0, uint32_t arg1)
{
	lprintf("task2\n");
	for (;;) {
		Board_LED_Set(1, 0);
		task_tsleep(MSEC(500));
		Board_LED_Set(1, 1);
		task_tsleep(MSEC(500));
	}
}

TaskCreateInfo	task_info[] = {
		{"TASK1", TASK_ACT, task1, 0, 1024, 1024, 5, (void*)128},
		{"TASK2", TASK_ACT, task2, 0, 1024, 1024, 6, (void*)128},
		{"TASK3", TASK_ACT, task1, 0, 1024, 1024, 5, (void*)128},
		{"TASK4", TASK_ACT, task2, 0, 1024, 1024, 6, (void*)128},
		{"TASK5", TASK_ACT, task1, 0, 1024, 1024, 5, (void*)128},
		{"TASK6", TASK_ACT, task2, 0, 1024, 1024, 6, (void*)128},
		{"TASK7", TASK_ACT, task1, 0, 1024, 1024, 5, (void*)128},
		{"TASK8", TASK_ACT, task2, 0, 1024, 1024, 6, (void*)128},
};

void main_task(void)
{
	int ix;
	for ( ix=0; ix<arrayof(task_info); ix++ ) {
		task_struct[ix] = task_create(&task_info[ix]);
	}

	task_sleep();
}
