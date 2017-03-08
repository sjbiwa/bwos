#include <string.h>
#include "bwos.h"
#include "ff.h"
#include "gicv3reg.h"

extern "C" {
extern void jpeg_decompress(void* image, size_t image_size, void* output, uint32_t* r_width, uint32_t* r_height);
}

/* configuration task */
static int		task_struct[100];
static int		fs_mutex;

static uint32_t file_load(const char* fname, void* buff)
{
	FIL fil;
	FILINFO stat;
	unsigned int bytes;
	mutex_lock(fs_mutex);
	if ( f_stat(fname, &stat) == FR_OK ) {
		f_open(&fil, fname, FA_READ);
		f_read(&fil, buff, stat.fsize, &bytes);
		f_close(&fil);
	}
	else {
		stat.fsize = 0;
	}
	mutex_unlock(fs_mutex);
	return stat.fsize;
}

static void jpeg_test(void* arg0, void* arg1)
{
	uint32_t cpuid = CPUID_get();
	uint32_t* tgt = (uint32_t*)(FRAME_BUFFER_BASE);
	tgt = (cpuid & 0x1) ? tgt + 512 : tgt;
	tgt = (cpuid & 0x2) ? tgt + 1024*384 : tgt;
	uint32_t width, height;
	void* src_image = sys_malloc(100000);
	void* dst_image = sys_malloc(1024*768*4);
	if ( !src_image || !dst_image ) {
		lprintf("alloc error\n");
	}

	for ( uint32_t fid = 1;; fid++ ) {
		if ( 900 < fid ) {
			fid = 1;
		}
		char fname[16];
		tsprintf(fname, "jpg/%04d.jpg", fid);
		uint32_t size = file_load(fname, src_image);
		if ( 0 < size ) {
			width = height = 0;
			jpeg_decompress(src_image, size, dst_image, &width, &height);
			for ( uint32_t iy = 0; iy < height; iy++ ) {
				for ( uint32_t ix = 0; ix < width; ix++ ) {
					uint8_t* ptr = (uint8_t*)dst_image;
					uint32_t ptn;
					ptn  = ((uint32_t)ptr[(iy*width+ix)*3+0] << 0);
					ptn |= ((uint32_t)ptr[(iy*width+ix)*3+1] << 8);
					ptn |= ((uint32_t)ptr[(iy*width+ix)*3+2] << 16);
					tgt[iy*1024+ix] = ptn;
				}
			}
		}
	}
}

static void test(void* arg0, void* arg1)
{
	uint32_t cpuid = CPUID_get();
	for (;;) {
		lprintf("task:%d\n", cpuid);
		void* ptr = sys_malloc_rgn(100, SYS_MALLOC_REGION_HIGH);
		lprintf("%d:%08X%08X\n", cpuid, hword(ptr), lword(ptr));
		task_tsleep(SEC(1));
	}
}


TaskCreateInfo	task_info[] = {
#if 0
		{"TASK1",  CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, jpeg_test, 65536, 0, 5, (void*)0},
		{"TASK2",  CPU_CORE1|TASK_ACT|TASK_FPU|TASK_SYS, jpeg_test, 65536, 0, 5, (void*)0},
		{"TASK3",  CPU_CORE2|TASK_ACT|TASK_FPU|TASK_SYS, jpeg_test, 65536, 0, 5, (void*)0},
		{"TASK4",  CPU_CORE3|TASK_ACT|TASK_FPU|TASK_SYS, jpeg_test, 65536, 0, 5, (void*)0},
#else
		{"TASK1",  CPU_CORE0|TASK_ACT|TASK_FPU|TASK_SYS, test, 65536, 0, 5, (void*)0},
		{"TASK2",  CPU_CORE1|TASK_ACT|TASK_FPU|TASK_SYS, test, 65536, 0, 5, (void*)0},
		{"TASK3",  CPU_CORE2|TASK_ACT|TASK_FPU|TASK_SYS, test, 65536, 0, 5, (void*)0},
		{"TASK4",  CPU_CORE3|TASK_ACT|TASK_FPU|TASK_SYS, test, 65536, 0, 5, (void*)0},
#endif
};

void main_task(void)
{
	int ix;
	lprintf("main is %d\n", CPUID_get());
	fs_mutex = mutex_create();
	FATFS fatfs;
	lprintf("f_mount:%d\n", f_mount(&fatfs, "0:", 1));

	for ( ix=0; ix<arrayof(task_info); ix++ ) {
		task_struct[ix] = task_create(&task_info[ix]);
	}
	task_sleep();
}

