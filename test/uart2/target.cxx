#include <string.h>
#include "bwos.h"
#include "driver/gpio.h"
#include "driver/video.h"
#include "driver/uart.h"
#include "cache.h"
#include "cp15reg.h"

extern "C" {
extern	void lprintf(...);
extern int tsprintf(char* buff,char* fmt, ...);
}

/* configuration task */
static int		task_struct[100];
static int		sync_flag;
static int		mutex;

static uint8_t make_checksum(void* buff, uint32_t length)
{
	uint8_t* p = static_cast<uint8_t*>(buff);
	uint8_t checksum = 0;
	for (; 0 < length; length--, p++ ) {
		checksum += *p;
	}
	return checksum;
}

void uart_test(void* arg0, void* arg1)
{
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

	uart_set_config(1, &config_param);
	uart_open(1, &open_param);

	/* 送信処理 */
	for (;;) {
		uint8_t buff[1024];
		/* NAK待ち */
		for (;;) {
			lprintf("NAK wait\n");
			uart_recv(1, buff, 1, TMO_FEVER);
			lprintf("recv %02X\n", buff[0]);
			if ( (buff[0] == 0x15) || (buff[0] == 'A') ) {
				lprintf("NAK recv\n");
				break;
			}
			else {
				lprintf("char recv\n");
			}
		}

		memset(buff, 0x55, sizeof(buff));
		for ( uint32_t block_num = 1; block_num < 1024; block_num++ ) {
			buff[0] = 0x01; /* SOH */
			buff[1] = block_num;
			buff[2] = ~block_num;
			uint8_t checksum = make_checksum(buff+3, 128);
			buff[3+128] = checksum;
			uart_send(1, buff, 3+128+1, TMO_FEVER);
			lprintf("send done:%d\n", block_num);
			uart_recv(1, buff, 1, TMO_FEVER);
			lprintf("RESP:%02X\n", buff[0]);
		}
		uart_send(1, (void*)"\x04", 1, TMO_FEVER);
		lprintf("done\n");
		task_sleep();
	}
}

TaskCreateInfo	task_info[] = {
		{"TASK1",  CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, uart_test, 65536, 0, 5, (void*)0},
};

void main_task(void)
{
	int ix;
	for ( ix=0; ix<arrayof(task_info); ix++ ) {
		task_struct[ix] = task_create(&task_info[ix]);
	}
	task_sleep();
}

