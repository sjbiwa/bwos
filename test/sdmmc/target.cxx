#include <string.h>
#include "bwos.h"
#include "driver/gpio.h"
#include "driver/video.h"
#include "ff.h"
#include "bitmap.h"

extern "C" {
extern	void lprintf(...);
}

class LedTest {
public:
		LedTest() : m_led_flag(false) {}
		void flush() {
			gpio_set_bit(8, 1, m_led_flag ? 1 : 0);
			m_led_flag = m_led_flag ? false : true;
		}

private:
		bool m_led_flag;
};


#define	STR_LEN		512
static uint8_t buff1[STR_LEN] __attribute__((aligned(CACHE_LINE_SIZE)));

/* configuration task */
static int		task_struct[1000];
static int		sync_flag;

static void task0(void* arg0, void* arg1)
{
	FATFS fatfs;      // File system object
	lprintf("f_mount:%d\n", f_mount(&fatfs, "0:", 1));

	Bitmap bmp[5];
	bmp[0].loadFile("demo.bmp");
	bmp[1].loadFile("image1.bmp");
	bmp[2].loadFile("image2.bmp");
	bmp[3].loadFile("image3.bmp");
	bmp[4].loadFile("image4.bmp");
	for (uint32_t ix=0;; ix++ ) {
		bmp[ix%5].drawImage((void*)0x20000000, 1024, 384, ix%256, ix%256);
		task_tsleep(MSEC(50));
	}
}

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

void task1(void* arg0, void* arg1)
{
	int ret;

	/* fill win0 */
	uint32_t addr = 0x20000000;
	uint32_t idx = 0;
	uint32_t col = 0;
	for (uint32_t ix=CPUID_get();;ix++) {
		task_set_affinity(ix % CPU_NUM);
		draw_fill_rectangle(addr, 512, 384, 1024, col, 1);
		col += 0x004488dd;
		uint32_t ret_pattern;
		flag_wait(sync_flag, 0x0001, FLAG_OR|FLAG_BITCLR, &ret_pattern);

	}
}

void task2(void* arg0, void* arg1)
{
	/* fill win1 */
	uint32_t addr = 0x20800000;
	uint32_t idx = 0;
	uint32_t col = 0x405060;
	for (uint32_t ix=CPUID_get();;ix++) {
		task_set_affinity(ix % CPU_NUM);
		draw_fill_rectangle(addr, 512, 384, 1024, col, 0x00FFEEDD);
		col += 0x00010306;
		uint32_t ret_pattern;
		flag_wait(sync_flag, 0x0002, FLAG_OR|FLAG_BITCLR, &ret_pattern);
	}
}

void task3(void* arg0, void* arg1)
{
	int ret;

	/* fill win0 */
	uint32_t addr = 0x21000000;
	uint32_t idx = 0;
	uint32_t col = 0;
	for (uint32_t ix=CPUID_get();;ix++) {
		task_set_affinity(ix % CPU_NUM);
		draw_fill_rectangle(addr, 512, 384, 1024, col, 0x80C0F0);
		col += 0x00010306;
		uint32_t ret_pattern;
		flag_wait(sync_flag, 0x0004, FLAG_OR|FLAG_BITCLR, &ret_pattern);
	}
}

void task4(void* arg0, void* arg1)
{
	/* fill win1 */
	uint32_t addr = 0x21800000;
	uint32_t idx = 0;
	uint32_t col = 0x405060;
	for (uint32_t ix=CPUID_get();;ix++) {
		task_set_affinity(ix % CPU_NUM);
		draw_fill_rectangle(addr, 512, 384, 1024, col, 0x010203);
		col += 0x00010306;
		uint32_t ret_pattern;
		flag_wait(sync_flag, 0x0008, FLAG_OR|FLAG_BITCLR, &ret_pattern);
	}
}

TaskCreateInfo	task_info[] = {
		{"TASK0", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task0, 8192, 0, 1},
//		{"TASK1", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, task1, 1024, 0, 5},
		{"TASK2", CPU_CORE1|TASK_ACT|TASK_FPU|TASK_SYS, task2, 1024, 0, 5},
		{"TASK3", CPU_CORE2|TASK_ACT|TASK_FPU|TASK_SYS, task3, 1024, 0, 5},
		{"TASK4", CPU_CORE3|TASK_ACT|TASK_FPU|TASK_SYS, task4, 1024, 0, 5},
};

extern "C" {

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
	video_register_handler(irq_handler);
	sync_flag = flag_create();
	sync_barrier();

	int ix;
	for ( ix=0; ix<arrayof(task_info); ix++ ) {
		task_struct[ix] = task_create(&task_info[ix]);
		lprintf("create %d\n", task_struct[ix]);
	}

	task_sleep();
}

};
