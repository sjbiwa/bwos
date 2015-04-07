/*
 * target.c
 *
 *  Created on: 2014/11/15
 *      Author: biwa
 */
#include "bwos.h"
#include "driver/uart.h"

/* configuration task */
int		task_struct[16];


static char buff[] = "Hello world\n";

void task1(void)
{
	int ret;
	int sent_bytes;
	UartConfigParam config_param = {
			UART_BAUD115200,
			UART_BITS_8,
			UART_PARITY_NONE,
			UART_STOPBIT_1,
			UART_FLOW_NONE
	};
	UartOpenParam open_param = {
			1024, 1024
	};
	uart_setConfig(1, &config_param);
	uart_open(1, &open_param);
	task_active(task_struct[1]);
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
		task_tsleep(MSEC(500));
	}
}

void task2(void)
{
	int ret;
	char buff[16];
	for (;;) {
		ret = uart_recv(1, buff, sizeof(buff)-1, SEC(5));
		if ( ret <= 0 ) {
			lprintf("recv error:%d\n", ret);
			task_tsleep(MSEC(100));
		}
		else {
			buff[ret] = '\0';
			lprintf("%s", buff);
		}
	}
}


TaskCreateInfo	task_info[] = {
		{"TASK1", TASK_ACT|TASK_FPU|TASK_SYS, task1, 0, 1024, 1024, 5},
		{"TASK2", TASK_FPU|TASK_SYS, task2, 0, 1024, 1024, 5},
};

void main_task(void)
{
	int ix;
	for ( ix=0; ix<arrayof(task_info); ix++ ) {
		task_struct[ix] = task_create(&task_info[ix]);
	}

	task_sleep();
}
