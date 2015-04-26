/*
 * target.c
 *
 *  Created on: 2014/11/15
 *      Author: biwa
 */
#include "bwos.h"
#include "driver/gpio.h"

/* configuration task */
int		task_struct[16];

void task1(void)
{
	int ret;

	gpio_set_direction(8, 0x00000006, 0x00000006); /* LED */
	gpio_set_direction(0, 0x00020000, 0x00020000); /* SDA7123 */
	gpio_set_bit(0, 17, 1);
	lprintf("video init\n");
	video_init();

	uint32_t col = 0;
	for ( col=0;; col++ ) {
		video_set_background(col);
		task_tsleep(MSEC(10));
	}
}

TaskCreateInfo	task_info[] = {
		{"TASK1", TASK_ACT|TASK_FPU|TASK_SYS, task1, 0, 1024, 1024, 5},
};

void main_task(void)
{
	int ix;
	for ( ix=0; ix<arrayof(task_info); ix++ ) {
		task_struct[ix] = task_create(&task_info[ix]);
	}

	task_sleep();
}
