#include <string.h>
#include "bwos.h"
#include "driver/gpio.h"
#include "driver/video.h"
#include "ff.h"
#include "cache.h"
#include "cp15reg.h"

extern "C" {
extern	void lprintf(...);
extern int tsprintf(char* buff,char* fmt, ...);
extern void jpeg_decompress(void* image, size_t image_size, void* output, uint32_t* r_width, uint32_t* r_height);
}

#define	MAX_FILE_NUM			(2000)

/* configuration task */
static int		task_struct[100];

static uint8_t buffer[1024*768*3];

static void*	image_addr[MAX_FILE_NUM];
static uint32_t	image_size[MAX_FILE_NUM];
static uint32_t	image_num = 0;

static void display_init()
{
	memset((void*)0x60000000, 0x00, 0x800000*4);
	video_init();
	video_layer_init(VIDEO_LAYER_0, 0x60000000);
	video_layer_init(VIDEO_LAYER_1, 0x60000000);
	video_layer_init(VIDEO_LAYER_2, 0x60000000);
	video_layer_init(VIDEO_LAYER_3, 0x60000000);
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
	display_init();

	FATFS fatfs;
	lprintf("f_mount:%d\n", f_mount(&fatfs, "0:", 1));
	files_load();

	for (uint32_t counter=0;; counter++ ) {
		uint32_t width, height;
		width = 1024;
		height = 768;
		jpeg_decompress(image_addr[counter%image_num], image_size[counter%image_num], buffer, &width, &height);
		uint8_t* src = buffer;
		for ( int iy=0; iy < height; iy++ ) {
			uint32_t* dest = (uint32_t*)(0x60000000 + iy * 1024 * 4);
			for ( int ix=0; ix < width; ix++ ) {
				uint32_t value = (src[0]<<16) | (src[1]<<8) | src[2];
				dest[0] = value;
				dest++;
				src += 3;
			}
		}
	}
}


TaskCreateInfo	task_info[] = {
		{"TASK1", CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, jpeg_test, 65536, 0, 5, (void*)1},
};

void main_task(void)
{
	for ( int ix=0; ix<arrayof(task_info); ix++ ) {
		task_struct[ix] = task_create(&task_info[ix]);
	}

	task_sleep();
}

