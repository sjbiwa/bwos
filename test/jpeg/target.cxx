#include <string.h>
#include "bwos.h"
#include "driver/gpio.h"
#include "driver/video.h"
#include "ff.h"
#include "cache.h"
#include "cp15reg.h"

extern "C" {
extern void jpeg_decompress(void* image, size_t image_size, void* output, uint32_t* r_width, uint32_t* r_height);
}

#define	MAX_FILE_NUM			(2000)
#define	BUFFER_SIZE				(1024*768*3)

typedef	struct {
	uint32_t	own_id;
	uint32_t	frame_buffer;
	uint32_t	draw_counter;
	uint32_t	prev_counter;
	int			pos_x;
	int			pos_y;
	int			diff_x;
	int			diff_y;
	uint8_t*	buffer;
} ExecInfo;

static ExecInfo	exec_info[4] = {
		{0, 0x60000000, 0, 0,   0,   0, 1, 1, NULL},
		{1, 0x60800000, 0, 0, 512,   0, 2, 3, NULL},
		{2, 0x61000000, 0, 0,   0, 384, 3, 2, NULL},
		{3, 0x61800000, 0, 0, 512, 384, 3, 3, NULL},
};
/* configuration task */
static int		task_struct[100];
static int		sync_flag;
static int		mutex;

static void*	image_addr[MAX_FILE_NUM];
static uint32_t	image_size[MAX_FILE_NUM];
static uint32_t	image_num = 0;

static void display_init()
{
	memset((void*)0x60000000, 0x00, 0x800000*4);
	video_init();
	video_layer_init(VIDEO_LAYER_0, 0x60000000);
	video_layer_init(VIDEO_LAYER_1, 0x60800000);
	video_layer_init(VIDEO_LAYER_2, 0x61000000);
	video_layer_init(VIDEO_LAYER_3, 0x61800000);
	video_layer_set(VIDEO_LAYER_0, 0,     0, 512, 384, 0, 1024);
	video_layer_set(VIDEO_LAYER_1, 512,   0, 512, 384, 0, 1024);
	video_layer_set(VIDEO_LAYER_2, 0,   384, 512, 384, 0, 1024);
	video_layer_set(VIDEO_LAYER_3, 512, 384, 512, 384, 0, 1024);
}

static uint32_t file_load(const char* fname, void* buff)
{
	FIL fil;
	FILINFO stat;
	unsigned int bytes;
	if ( f_stat(fname, &stat) == FR_OK ) {
		f_open(&fil, fname, FA_READ);
		f_read(&fil, buff, stat.fsize, &bytes);
		f_close(&fil);
	}
	else {
		stat.fsize = 0;
	}
	return stat.fsize;
}

static void files_load()
{
	uint8_t* buff_addr = (uint8_t*)sys_malloc_align(100000000, 16);
	char fname[16];
	for ( image_num = 0; image_num < MAX_FILE_NUM; image_num++ ) {
		tsprintf(fname, "jpg/%04d.jpg", image_num+1);
		if ( (image_num % 10) == 0 ) {
			lprintf("load:%s\n", fname);
		}
		uint32_t fsize = file_load(fname, buff_addr);
		if ( fsize == 0 ) {
			break;
		}
		image_addr[image_num] = buff_addr;
		image_size[image_num] = fsize;
		buff_addr += fsize;
		buff_addr = (uint8_t*)(POST_ALIGN_BY(buff_addr, 16));
	}
}

void jpeg_test(void* arg0, void* arg1)
{
	ExecInfo*	exec_info = static_cast<ExecInfo*>(arg0);
	uint32_t ret_pattern;
	flag_wait(sync_flag, 0x1, FLAG_OR, &ret_pattern);

	for (uint32_t counter=exec_info->own_id*100;; counter++ ) {
		uint32_t width, height;
		width = 1024;
		height = 768;
		jpeg_decompress(image_addr[counter%image_num], image_size[counter%image_num], exec_info->buffer, &width, &height);
		uint8_t* src = exec_info->buffer;
		for ( int iy=0; iy < height; iy++ ) {
			uint32_t* dest = (uint32_t*)(exec_info->frame_buffer + iy * 1024 * 4);
			for ( int ix=0; ix < width; ix++ ) {
				uint32_t value = 0xF0000000 | (src[0]<<16) | (src[1]<<8) | src[2];
				dest[0] = value;
				dest++;
				src += 3;
			}
		}
		exec_info->draw_counter++;
	}
}

extern "C" {
void dump_log();
}

static void survey_perf(void* arg0, void* arg1)
{
	(void)arg0, (void)arg1;
	display_init();
	FATFS fatfs;
	lprintf("f_mount:%d\n", f_mount(&fatfs, "0:", 1));
	files_load();

	flag_set(sync_flag, 0xffff);

	for (;;) {
		task_tsleep(SEC(10));
		lprintf("%d %d %d %d\n",
				exec_info[0].draw_counter,
				exec_info[1].draw_counter,
				exec_info[2].draw_counter,
				exec_info[3].draw_counter);
		void dump_log();
	}
	task_sleep();
}

static void change_position(void* arg0, void* arg1)
{
	uint32_t ret_pattern;
	flag_wait(sync_flag, 0x1, FLAG_OR, &ret_pattern);
	for (;;) {
		task_tsleep(MSEC(20));
		video_layer_set(VIDEO_LAYER_0, exec_info[0].pos_x, exec_info[0].pos_y, 512, 384, 0, 1024);
		video_layer_set(VIDEO_LAYER_1, exec_info[1].pos_x, exec_info[1].pos_y, 512, 384, 0, 1024);
		video_layer_set(VIDEO_LAYER_2, exec_info[2].pos_x, exec_info[2].pos_y, 512, 384, 0, 1024);
		video_layer_set(VIDEO_LAYER_3, exec_info[3].pos_x, exec_info[3].pos_y, 512, 384, 0, 1024);

		for (int ix=0; ix < 4; ix++ ) {
			exec_info[ix].pos_x += exec_info[ix].diff_x;
			exec_info[ix].pos_y += exec_info[ix].diff_y;
			if ( (exec_info[ix].pos_x < 0) || (512 < exec_info[ix].pos_x) ) {
				exec_info[ix].diff_x = -exec_info[ix].diff_x;
				exec_info[ix].pos_x += exec_info[ix].diff_x * 2;
			}
			if ( (exec_info[ix].pos_y < 0) || (384 < exec_info[ix].pos_y) ) {
				exec_info[ix].diff_y = -exec_info[ix].diff_y;
				exec_info[ix].pos_y += exec_info[ix].diff_y * 2;
			}
		}
	}
}

TaskCreateInfo	task_info[] = {
		/* 計測用 */
		{"SURVEY", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, survey_perf, 65536, 0, 1, (void*)0},
		/* 計測用 */
		{"CHANGE", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, change_position, 65536, 0, 1, (void*)0},
		/* JPEG描画用 */
		{"TASK1",  CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, jpeg_test, 65536, 0, 5, (void*)(&exec_info[0])},
		{"TASK2",  CPU_CORE1|TASK_ACT|TASK_FPU|TASK_SYS, jpeg_test, 65536, 0, 5, (void*)(&exec_info[1])},
		{"TASK3",  CPU_CORE2|TASK_ACT|TASK_FPU|TASK_SYS, jpeg_test, 65536, 0, 5, (void*)(&exec_info[2])},
		{"TASK4",  CPU_CORE3|TASK_ACT|TASK_FPU|TASK_SYS, jpeg_test, 65536, 0, 5, (void*)(&exec_info[3])},
};

void main_task(void)
{
	sync_flag = flag_create();
	mutex = mutex_create();
	int ix;
	for ( ix=0; ix<arrayof(exec_info); ix++ ) {
		exec_info[ix].buffer = static_cast<uint8_t*>(sys_malloc(BUFFER_SIZE));
	}
	for ( ix=0; ix<arrayof(task_info); ix++ ) {
		task_struct[ix] = task_create(&task_info[ix]);
	}
	task_sleep();
}

