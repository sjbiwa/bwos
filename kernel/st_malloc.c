/*
 * Copyright (C) 2015 sjbiwa All rights reserved.
 */

/*
 * st_malloc.c
 *
 *  Created on: 2015/02/28
 *      Author: biwa
 */

#include "kernel.h"

/* 起動直後にのみ使用されるメモリマネージャ */
/* 割り当てのみで開放はできない領域用 */
/* シングルスレッドで使うこと */

static void*	start_mem_addr;
static void*	end_mem_addr;

KERNAPI void __st_malloc_init(void* mem_addr, MemSize_t mem_size)
{
	start_mem_addr = mem_addr;
	end_mem_addr = (void*)(PTRVAR(mem_addr) + mem_size);
}

KERNAPI void* __st_malloc_align(MemSize_t alloc_size, uint32_t align)
{
	void* ret = POST_ALIGN_BY(start_mem_addr, align);
	start_mem_addr = PTRVAR(ret) + alloc_size;
	if ( end_mem_addr <= start_mem_addr ) {
		ret = 0;
	}
	return ret;
}

KERNAPI void __st_malloc_normalize(void)
{
	__sys_malloc_add_block(start_mem_addr, PTRVAR(end_mem_addr) - PTRVAR(start_mem_addr));
	start_mem_addr = end_mem_addr = 0;
}
