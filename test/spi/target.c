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
	chconfig.baudrate = 4000000;
	spi_set_channel_config(0, 0, &chconfig);
	chconfig.baudrate = 1000000;
	spi_set_channel_config(0, 1, &chconfig);

	task_active(task_struct[1]);
	task_active(task_struct[2]);

	task_sleep();
}

void task2(void)
{
	for (;;) {
		uint8_t tx_buff[] = { 0x1A, 0x2B, 0x3C, 0x4D, 0x5E, 0x6F };
		uint8_t rx_buff[64];
		memset(rx_buff, 0, sizeof(rx_buff));
		SpiTransferParam param;
		param.tx_buf = tx_buff;
		param.rx_buf = rx_buff;
		param.tx_length = sizeof(tx_buff);
		param.rx_length = 3;
		spi_transfer(0, 0, &param);
		lprintf("0:%02X %02X %02X %02X %02X %02X\n", rx_buff[0], rx_buff[1], rx_buff[2], rx_buff[3], rx_buff[4], rx_buff[5]);
	}
}

void task3(void)
{
	for (;;) {
		uint8_t tx_buff[] = { 0xAA, 0x99, 0x88, 0x77, 0x66, 0x55 };
		uint8_t rx_buff[64];
		memset(rx_buff, 0, sizeof(rx_buff));
		SpiTransferParam param;
		param.tx_buf = tx_buff;
		param.rx_buf = rx_buff;
		param.tx_length = sizeof(tx_buff);
		param.rx_length = 5;
		spi_transfer(0, 1, &param);
		lprintf("1:%02X %02X %02X %02X %02X %02X\n", rx_buff[0], rx_buff[1], rx_buff[2], rx_buff[3], rx_buff[4], rx_buff[5]);
	}
}

TaskCreateInfo	task_info[] = {
		{"TASK1", TASK_ACT|TASK_FPU|TASK_SYS, task1, 0, 1024, 1024, 5},
		{"TASK2", TASK_FPU|TASK_SYS, task2, 0, 1024, 1024, 6},
		{"TASK3", TASK_FPU|TASK_SYS, task3, 0, 1024, 1024, 6},
};

void main_task(void)
{
	int ix;
	for ( ix=0; ix<arrayof(task_info); ix++ ) {
		task_struct[ix] = task_create(&task_info[ix]);
	}

	task_sleep();
}
