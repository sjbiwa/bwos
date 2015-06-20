/*
 * target.c
 *
 *  Created on: 2014/11/15
 *      Author: biwa
 */
#include "bwos.h"
#include "driver/i2c.h"

/* configuration task */
static int		task_struct[16];

void task1(uint32_t arg0, uint32_t arg1)
{
	I2cPortConfig	config;
	I2cTransferMethod	method[6];
	I2cTransferParam param = { arrayof(method), method };

	config.baudrate = 1000;
	i2c_set_port_config(4, &config);
	i2c_transfer(4, &param);

	//task_active(task_struct[1], (void*)2048);
	//task_active(task_struct[2], (void*)4096);

	task_sleep();
}

void task2(uint32_t arg0, uint32_t arg1)
{
}

void task3(uint32_t arg0, uint32_t arg1)
{
}

TaskCreateInfo	task_info[] = {
		{"TASK1", TASK_ACT|TASK_FPU|TASK_SYS, task1, 0, 1024, 1024, 5, (void*)128},
		{"TASK2", TASK_FPU|TASK_SYS, task2, 0, 1024, 1024, 6, (void*)256},
		{"TASK3", TASK_FPU|TASK_SYS, task3, 0, 1024, 1024, 6, (void*)512},
};

void main_task(void)
{
	int ix;
	for ( ix=0; ix<arrayof(task_info); ix++ ) {
		task_struct[ix] = task_create(&task_info[ix]);
	}

	task_sleep();
}
