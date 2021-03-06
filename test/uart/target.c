/*
 * target.c
 *
 *  Created on: 2014/11/15
 *      Author: biwa
 */
#include "bwos.h"
#include "driver/uart.h"
#include "driver/gpio.h"

/* configuration task */
int		task_struct[16];


static char buff[] = "Hello world1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz\n";

static uint32_t counter = 0;

void task1(void)
{
	int ret;
	int sent_bytes;
	UartConfigParam config_param = {
			UART_BAUD9600,
			UART_BITS_7,
			UART_PARITY_ODD,
			UART_STOPBIT_1,
			UART_FLOW_NONE
	};
	UartOpenParam open_param = {
			256, 256
	};
	gpio_set_direction(8, 0x00000006, 0x00000006); /* LED */
	gpio_set_direction(0, 0x02000000, 0x02000000); /* SDA7123 */
	gpio_set_bit(0, 1, 1);

	task_tsleep(MSEC(100));
	lprintf("video init\n");
	video_init();
	uart_set_config(1, &config_param);
	uart_open(1, &open_param);
	task_active(task_struct[1], 0);
	task_active(task_struct[2], 0);

	/* 送信処理 */
	for (;;) {
		sent_bytes = 0;
		while ( sent_bytes < sizeof(buff) ) {
			ret = uart_send(1, buff+sent_bytes, sizeof(buff)-sent_bytes, TMO_FEVER);
			if ( ret <= 0 ) {
				lprintf("send error:%d\n", ret);
			}
			else {
				sent_bytes += ret;
			}
		}
		task_tsleep(MSEC(100));
		counter++;
		if ( counter & 0x01 ) {
			gpio_set_bit(8, 1, 1);
		}
		else {
			gpio_set_bit(8, 1, 0);
		}
	}
}

void task2(void)
{
	int ret;
	char buff[16];
	uint32_t count = 0;

	/* 受信処理 */
	for (;;) {
		ret = uart_recv(1, buff, sizeof(buff)-1, SEC(1));
		if ( ret <= 0 ) {
			count++;
			if ( count & 1 ) {
				gpio_set_bit(8, 2, 1);
			}
			else {
				gpio_set_bit(8, 2, 0);
			}
			task_tsleep(MSEC(100));
		}
		else {
			buff[ret] = '\0';
			lprintf("%s", buff);
		}
	}
}

void task3(void)
{
	int ret;
	uint32_t error;
	double d;

	d = 10.0;
	/* エラー処理 */
	for (;;) {
		d *= 12.3 + 3.3;
		ret = uart_error(1, &error, TMO_FEVER);
		if ( ret < 0 ) {
			task_tsleep(MSEC(100));
		}
		else {
			lprintf("UART error:%08X\n", error);
		}
	}
}


TaskCreateInfo	task_info[] = {
		{"TASK1", TASK_ACT|TASK_FPU|TASK_SYS, task1, 1024, 1024, 5},
		{"TASK2", TASK_FPU|TASK_SYS, task2, 1024, 1024, 5},
		{"TASK3", TASK_FPU|TASK_SYS, task3, 1024, 1024, 5},
};

void main_task(void)
{
	int ix;
	for ( ix=0; ix<arrayof(task_info); ix++ ) {
		task_struct[ix] = task_create(&task_info[ix]);
	}

	task_sleep();
}
