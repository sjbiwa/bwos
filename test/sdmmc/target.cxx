#include "bwos.h"
#include "ff.h"
#include "driver/gpio.h"

extern "C" {
extern	void lprintf(...);
}

/* configuration task */
static int		task_struct[100];

#define	STR_LEN		512
static uint8_t buff1[STR_LEN] __attribute__((aligned(CACHE_LINE_SIZE)));
static uint8_t buff2[STR_LEN] __attribute__((aligned(CACHE_LINE_SIZE)));

static bool led_flag = false;

static void led_flush(void)
{
	gpio_set_bit(8, 1, led_flag?1:0);
	led_flag = led_flag?false:true;
}

static void snap(uint32_t* buff, uint32_t length)
{
	uint8_t text[STR_LEN+1];
	text[STR_LEN] = '\0';
	uint8_t* ptr = (uint8_t*)buff;
	for ( uint32_t ix=0; ix < length; ix++ ) {
		lprintf("%02X ", ptr[ix]);
#if 0
		if ( (0x20 <= ptr[ix]) && (ptr[ix] <= 0x7e) ) {
			text[ix%STR_LEN] = ptr[ix];
		}
		else {
			text[ix%STR_LEN] = '.';
		}
		if ( (ix % STR_LEN) == (STR_LEN-1) ) {
			lprintf(" : %s\n", text);
		}
#endif
	}
	lprintf("\n");
}

static void task1(void* arg0, void* arg1)
{
	FATFS fatfs;      // File system object
	lprintf("f_mount:%d\n", f_mount(&fatfs, "0:", 1));

	while (1) {
		FIL fil;
		unsigned int bytes;
		uint32_t total = 0;
		f_open(&fil, "ff.c", FA_READ);
		while ( (f_read(&fil, buff1, 64, &bytes) == FR_OK) && (0 < bytes) ) {
			total += bytes;
			buff1[bytes] = '\0';
			lprintf("%s", buff1);
		}
		f_close(&fil);
		lprintf("\ncomplete:%d\n", total);
		task_tsleep(SEC(1));
	}
}

static void task2(void* arg0, void* arg1)
{
	for (;;) {
		led_flush();
		task_tsleep(SEC(1));
	}
}

TaskCreateInfo	task_info[] = {
		{(uint8_t*)("TASK01"), CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task1, 16000, 0, 5, (void*)0},
		{(uint8_t*)("TASK02"), CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task2, 1024, 0, 1, (void*)0},
};

extern "C" {

void main_task(void)
{
	int ix;
	for ( ix=0; ix<arrayof(task_info); ix++ ) {
		task_struct[ix] = task_create(&task_info[ix]);
	}

	task_sleep();
}

};
