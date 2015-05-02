/*
 * target.c
 *
 *  Created on: 2014/11/15
 *      Author: biwa
 */
#include "bwos.h"
#include "driver/gpio.h"
#include "driver/video.h"

/* configuration task */
static int		task_struct[16];
static int		sync_flag;

OSAPI int flag_set(int id, uint32_t pattern);
OSAPI int flag_wait(int id, uint32_t pattern, uint32_t wait_mode, uint32_t* ret_pattern);

static void irq_handler(uint32_t event)
{
	if ( event & VIDEO_IRQ_FRAME_END ) {
		flag_set(sync_flag, 0x000f);
	}
}

static void draw_rectangle(uint32_t base_addr, uint32_t width, uint32_t height, uint32_t stride, uint32_t col)
{
	uint32_t posx, posy;
	/* 横線 */
	for ( posx = 0; posx < width; posx++ ) {
		((volatile uint32_t*)(base_addr))[posx] = col;
		((volatile uint32_t*)(base_addr))[(height-1)*stride+posx] = col;
	}
	/* 縦線 */
	for ( posy = 0; posy < height; posy++ ) {
		((volatile uint32_t*)(base_addr))[posy*stride] = col;
		((volatile uint32_t*)(base_addr))[posy*stride+(width-1)] = col;
	}
}

static void draw_fill_rectangle(uint32_t base_addr, uint32_t width, uint32_t height, uint32_t stride, uint32_t start_col, uint32_t step_col)
{
	uint32_t ix;
	uint32_t max_ix = MIN(width, height) / 2;
	for ( ix=0; ix<max_ix; ix++ ) {
		draw_rectangle(base_addr, width, height, stride, start_col);
		base_addr += (stride+1)*4;
		width -= 2;
		height -= 2;
		start_col += step_col;
	}
}

void task1(void)
{
	int ret;

	sync_flag = flag_create();
	video_register_handler(irq_handler);

	/* fill win0 */
	uint32_t addr = 0x20000000;
	uint32_t idx = 0;
	uint32_t col = 0;
	for (;;) {
		draw_fill_rectangle(addr, 512, 384, 1024, col, 1);
		col += 0x004488dd;
		uint32_t ret_pattern;
		flag_wait(sync_flag, 0x0001, FLAG_OR|FLAG_BITCLR, &ret_pattern);
	}
}

void task2(void)
{
	/* fill win1 */
	uint32_t addr = 0x20800000;
	uint32_t idx = 0;
	uint32_t col = 0x405060;
	for (;;) {
		draw_fill_rectangle(addr, 512, 384, 1024, col, 0x00FFEEDD);
		col += 0x00010306;
		uint32_t ret_pattern;
		flag_wait(sync_flag, 0x0002, FLAG_OR|FLAG_BITCLR, &ret_pattern);
	}
}

void task3(void)
{
	int ret;

	/* fill win0 */
	uint32_t addr = 0x21000000;
	uint32_t idx = 0;
	uint32_t col = 0;
	for (;;) {
		draw_fill_rectangle(addr, 512, 384, 1024, col, 0x80C0F0);
		col += 0x00010306;
		uint32_t ret_pattern;
		flag_wait(sync_flag, 0x0004, FLAG_OR|FLAG_BITCLR, &ret_pattern);
	}
}

void task4(void)
{
	/* fill win1 */
	uint32_t addr = 0x21800000;
	uint32_t idx = 0;
	uint32_t col = 0x405060;
	for (;;) {
		draw_fill_rectangle(addr, 512, 384, 1024, col, 0x010203);
		col += 0x00010306;
		uint32_t ret_pattern;
		flag_wait(sync_flag, 0x0008, FLAG_OR|FLAG_BITCLR, &ret_pattern);
	}
}

TaskCreateInfo	task_info[] = {
		{"TASK1", TASK_ACT|TASK_FPU|TASK_SYS, task1, 0, 1024, 1024, 5},
		{"TASK2", TASK_ACT|TASK_FPU|TASK_SYS, task2, 0, 1024, 1024, 5},
		{"TASK3", TASK_ACT|TASK_FPU|TASK_SYS, task3, 0, 1024, 1024, 5},
		{"TASK4", TASK_ACT|TASK_FPU|TASK_SYS, task4, 0, 1024, 1024, 5},
};

void main_task(void)
{
	gpio_set_direction(8, 0x00000006, 0x00000006); /* LED */
	gpio_set_direction(0, 0x00020000, 0x00020000); /* SDA7123 */
	gpio_set_bit(0, 17, 1);
	memset((void*)0x20000000, 0x00, 0x800000*4);
	video_init();
	video_layer_init(VIDEO_LAYER_0, 0x20000000);
	video_layer_init(VIDEO_LAYER_1, 0x20800000);
	video_layer_init(VIDEO_LAYER_2, 0x21000000);
	video_layer_init(VIDEO_LAYER_3, 0x21800000);
	video_layer_set(VIDEO_LAYER_0, 0,     0, 512, 384, 0, 1024);
	video_layer_set(VIDEO_LAYER_1, 512,   0, 512, 384, 0, 1024);
	video_layer_set(VIDEO_LAYER_2, 0,   384, 512, 384, 0, 1024);
	video_layer_set(VIDEO_LAYER_3, 512, 384, 512, 384, 0, 1024);
	lprintf("video init\n");

	int ix;
	for ( ix=0; ix<arrayof(task_info); ix++ ) {
		task_struct[ix] = task_create(&task_info[ix]);
	}

	task_sleep();
}
