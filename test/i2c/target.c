/*
 * target.c
 *
 *  Created on: 2014/11/15
 *      Author: biwa
 */
#include "bwos.h"
#include "driver/i2c.h"
#include "cp15reg.h"

/* configuration task */
static int		task_struct[16];

void task1(uint32_t arg0, uint32_t arg1)
{
	I2cPortConfig	config;

	config.baudrate = 1000;
	i2c_set_port_config(4, &config);

	task_active(task_struct[1], (void*)2048);
	task_active(task_struct[2], (void*)4096);

	task_sleep();
}

void task2(uint32_t arg0, uint32_t arg1)
{
	uint8_t	buff[90];
	I2cTransferMethod	method[] = {
			{.mode = I2C_TX_MODE, .addr = 0x23, .bits = I2C_7BIT, .buff = (uint8_t[]){0x10}, .length = 1},
			{.mode = I2C_RX_MODE, .addr = 0x23, .bits = I2C_7BIT, .buff = buff, .length = 2},
			{.mode = I2C_TX_MODE, .addr = 0x23, .bits = I2C_7BIT, .buff = buff, .length = 90},
			{.mode = I2C_RX_MODE, .addr = 0x23, .bits = I2C_7BIT, .buff = buff, .length = 2},
			{.mode = I2C_RX_MODE, .addr = 0x23, .bits = I2C_7BIT, .buff = buff, .length = 2},
	};
	I2cTransferParam param[] = {
			{ 1, &method[0]},
			{ 1, &method[1]},
			{ 1, &method[2]},
			{ 2, &method[3]},
	};
	for ( int ix=0; ix < sizeof(buff); ix++ ) {
		buff[ix] = ix+0x10;
	}

	int ret = i2c_transfer(4, &param[0], TMO_FEVER);
	lprintf("transfer:ret=%d:%02X:%02X\n", ret, buff[0], buff[1]);
	for (;;) {
		ret = i2c_transfer(4, &param[1], TMO_FEVER);
		lprintf("0x23 transfer:ret=%d:%02X:%02X\n", ret, buff[0], buff[1]);
		task_tsleep(MSEC(100));
	}
}

void task3(uint32_t arg0, uint32_t arg1)
{
	uint8_t	buff[64];
	I2cTransferMethod	method[] = {
			{.mode = I2C_TX_MODE, .addr = 0x77, .bits = I2C_7BIT, .buff = (uint8_t[]){0xF6}, .length = 1},
			{.mode = I2C_RX_MODE, .addr = 0x77, .bits = I2C_7BIT, .buff = buff, .length = 2},
			{.mode = I2C_TX_MODE, .addr = 0x77, .bits = I2C_7BIT, .buff = (uint8_t[]){0xD0}, .length = 1},
			{.mode = I2C_RX_MODE, .addr = 0x77, .bits = I2C_7BIT, .buff = buff, .length = 2},
	};
	I2cTransferParam param[] = {
			{ 2, &method[0]},
			{ 2, &method[2]},
	};

	for (;;) {
		int ret = i2c_transfer(4, &param[arg0], TMO_FEVER);
		lprintf("%d:0x77 transfer:ret=%d:%02X:%02X\n", arg0, ret, buff[0], buff[1]);
		task_tsleep(MSEC(100));
	}
}

TaskCreateInfo	task_info[] = {
		{"TASK1", TASK_ACT|TASK_FPU|TASK_SYS, task1, 0, 1024, 1024, 5, (void*)128},
		{"TASK2", TASK_FPU|TASK_SYS, task3, 0, 1024, 1024, 6, (void*)0},
		{"TASK3", TASK_FPU|TASK_SYS, task3, 0, 1024, 1024, 6, (void*)1},
};

void main_task(void)
{
	int ix;
	for ( ix=0; ix<arrayof(task_info); ix++ ) {
		task_struct[ix] = task_create(&task_info[ix]);
	}

	task_sleep();
}
