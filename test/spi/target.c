/*
 * target.c
 *
 *  Created on: 2014/11/15
 *      Author: biwa
 */
#include "bwos.h"
#include "driver/spi.h"
#include "cp15reg.h"

/* configuration task */
static int		task_struct[16];

void task1(uint32_t arg0, uint32_t arg1)
{
	SpiPortConfig pconfig;
	SpiChannelConfig chconfig;
	SpiTransferParam param;

	spi_set_port_config(0, &pconfig);
	chconfig.ch_no = 0;
	chconfig.baudrate = 100000;
	chconfig.bits = 8;
	chconfig.endian = SPI_ENDIAN_LITTLE;
	chconfig.firstbit = SPI_FIRSTBIT_MSB;
	chconfig.scpol = SPI_SCPOL_HIGH;
	chconfig.scph = SPI_SCPH_START;
	spi_set_channel_config(0, &chconfig);

	chconfig.ch_no = 1;
	chconfig.baudrate = 48000000;
	chconfig.bits = 8;
	spi_set_channel_config(0, &chconfig);

	for ( int ix=1; ix < 8; ix++ ) {
		task_active(task_struct[ix], (void*)2048);
	}

	task_sleep();
}

void task2(uint32_t arg0, uint32_t arg1)
{
	SpiTransferParam param;
	uint8_t tx_buff[3];
	uint8_t rx_buff[64];
	memset(rx_buff, 0, sizeof(rx_buff));

	lprintf("task2:%d %d\n", arg0, arg1);
	tx_buff[0] = 0x58;
	param.ch_no = 0;
	param.tx_buf = tx_buff;
	param.rx_buf = rx_buff;
	param.tx_length = 1;
	param.rx_length = 2;
	spi_transfer(0, &param);
	lprintf("ID=%02X\n", rx_buff[1]);

	tx_buff[0] = 0x50;
	for (;;) {
		memset(rx_buff, 0, sizeof(rx_buff));
		param.tx_length = 1;
		param.rx_length = 3;
		spi_transfer(0, &param);
		double temp = (rx_buff[1]<<8|rx_buff[2]);
		temp /= 128;
		lprintf("0:%d.%02d ℃ (CORE=%d)\n", (int)temp, (int)((temp-(int)temp)*100), CPUID_get());
		task_tsleep(MSEC(100));
	}
}

void task3(uint32_t arg0, uint32_t arg1)
{
	lprintf("task3:%d %d\n", arg0, arg1);
	for (;;) {
		uint8_t tx_buff[] = { 0x08, 0x99, 0x88, 0x77, 0x66, 0x55 };
		uint8_t rx_buff[64];
		memset(rx_buff, 0, sizeof(rx_buff));
		SpiTransferParam param;
		param.ch_no = 0;
		param.tx_buf = tx_buff;
		param.rx_buf = rx_buff;
		param.tx_length = sizeof(tx_buff);
		param.rx_length = 5;
		spi_transfer(0, &param);
		lprintf("1:%02X %02X %02X %02X %02X %02X\n", rx_buff[0], rx_buff[1], rx_buff[2], rx_buff[3], rx_buff[4], rx_buff[5]);
		task_tsleep(SEC(3));
	}
}

TaskCreateInfo	task_info[] = {
		{"TASK1", TASK_ACT|TASK_FPU|TASK_SYS, task1, 1024, 1024, 5, (void*)128},
		{"TASK2", CPU_CORE0|TASK_FPU|TASK_SYS, task2, 1024, 1024, 6, (void*)256},
		{"TASK3", CPU_CORE1|TASK_FPU|TASK_SYS, task2, 1024, 1024, 6, (void*)512},
		{"TASK2", CPU_CORE2|TASK_FPU|TASK_SYS, task2, 1024, 1024, 6, (void*)256},
		{"TASK3", CPU_CORE3|TASK_FPU|TASK_SYS, task2, 1024, 1024, 6, (void*)512},
		{"TASK2", CPU_CORE0|TASK_FPU|TASK_SYS, task2, 1024, 1024, 6, (void*)256},
		{"TASK3", CPU_CORE1|TASK_FPU|TASK_SYS, task2, 1024, 1024, 6, (void*)512},
		{"TASK2", CPU_CORE2|TASK_FPU|TASK_SYS, task2, 1024, 1024, 6, (void*)256},
		{"TASK3", CPU_CORE3|TASK_FPU|TASK_SYS, task2, 1024, 1024, 6, (void*)512},
};

void main_task(void)
{
	int ix;
	for ( ix=0; ix<arrayof(task_info); ix++ ) {
		task_struct[ix] = task_create(&task_info[ix]);
	}

	task_sleep();
}
