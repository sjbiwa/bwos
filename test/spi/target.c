/*
 * target.c
 *
 *  Created on: 2014/11/15
 *      Author: biwa
 */
#include "bwos.h"
#include "driver/spi.h"

/* configuration task */
static int		task_struct[16];


void task1(void)
{
	SpiPortConfig pconfig;
	SpiChannelConfig chconfig;
	SpiTransferParam param;

	spi_set_port_config(0, &pconfig);
	for (;;) {
		spi_transfer(0, 0, &param);
		task_tsleep(MSEC(100));
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
