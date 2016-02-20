#include "bwos.h"

/* configuration task */
static int		task_struct[100];

#define	STR_LEN		128
static uint32_t buff[512/4] __attribute__((aligned(CACHE_LINE_SIZE)));

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
		//lprintf("%02X ", ptr[ix]);
		if ( (0x20 <= ptr[ix]) && (ptr[ix] <= 0x7e) ) {
			text[ix%STR_LEN] = ptr[ix];
		}
		else {
			text[ix%STR_LEN] = '.';
		}
		if ( (ix % STR_LEN) == (STR_LEN-1) ) {
			lprintf(" : %s\n", text);
		}
	}
	lprintf("\n");
}

void task1(uint32_t arg0, uint32_t arg1)
{
	lprintf("CORE=%d:task%d\n", CPUID_get(), arg0);
	sdmmc_init(0);
	uint32_t sect_num = sdmmc_get_sector_num(0);
	for (uint32_t sect = 0; sect < sect_num; sect++ ) {
		sdmmc_read_sector(0, sect, buff);
		snap(buff, 512);
	}
}

void task2(uint32_t arg0, uint32_t arg1)
{
	for (;;) {
		led_flush();
		task_tsleep(SEC(1));
	}
}

TaskCreateInfo	task_info[] = {
		{"TASK01", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task1, 1024, 0, 5, (void*)0},
		{"TASK02", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task2, 1024, 0, 1, (void*)0},
};

void main_task(void)
{
	int ix;
	for ( ix=0; ix<arrayof(task_info); ix++ ) {
		task_struct[ix] = task_create(&task_info[ix]);
	}

	task_sleep();
}
