/*
 * target.c
 *
 *  Created on: 2014/11/15
 *      Author: biwa
 */
#include "bwos.h"
#include "board_api.h"


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

void tmout_handler(void* param, bool is_first)
{
static bool flag = true;
	Board_LED_Set(0, flag ? 1 : 0);
	flag = flag ? false : true;
}

void timer_start()
{
	TimerInfo info;
	int timer = timer_create();
	info.cyclic = MSEC(200);
	info.tmout = SEC(5);
	info.kind = TIMER_CYCLIC;
	info.param = 0;
	info.handler = tmout_handler;

	timer_set(timer, &info);
	timer_enable(timer, true);
}

void task1(void* arg0, void* arg1)
{
	lprintf("task1\n");
	for (;;) {
		Board_LED_Set(0, 0);
		task_tsleep(MSEC(100));
		Board_LED_Set(0, 1);
		task_tsleep(MSEC(100));
	}
}

void task2(void* arg0, void* arg1)
{
	lprintf("task2\n");
	for (;;) {
		Board_LED_Set(1, 0);
		task_tsleep(SEC(2));
		Board_LED_Set(1, 1);
		task_tsleep(SEC(2));
	}
}

float a = 10.0f;
float b = 20.0f;

void task3(void* arg0, void* arg1)
{
	lprintf("task3\n");
	for (;;) {
		a += 1.1f;
		lprintf("task3:%d\n", (uint32_t)a);
		task_tsleep(MSEC(10));
	}
}

void task4(uint32_t arg0, uint32_t arg1)
{
	lprintf("task4\n");
	for (;;) {
		b += 2.2f;
		lprintf("task4:%d\n", (uint32_t)b);
		task_tsleep(MSEC(10));
	}
}

TaskCreateInfo	task_info[] = {
//		{"TASK1", TASK_ACT, task1, 1024, 1024, 5, (void*)128},
		{"TASK2", TASK_ACT, task2, 1024, 1024, 6, (void*)128},
//		{"TASK3", TASK_ACT, task3, 1024, 1024, 7, (void*)128},
//		{"TASK4", TASK_ACT, task4, 1024, 1024, 8, (void*)128},
};

void main_task(void)
{
	int ix;
	for ( ix=0; ix<arrayof(task_info); ix++ ) {
		task_struct[ix] = task_create(&task_info[ix]);
	}
	timer_start();

	task_sleep();
}
