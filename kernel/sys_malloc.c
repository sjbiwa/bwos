/*
 * Copyright (C) 2015 sjbiwa All rights reserved.
 */

/*
 * sys_malloc.c
 *
 *  Created on: 2016/04/08
 *      Author: biwa
 */

#include "kernel.h"

/* 空きメモリブロック先頭 */
#if !defined(SYS_MALLOC_REGION_NUM)
#define	MB_SPACE_RGN_NUM		1
#define	MB_LINK_TOP(n)			(&mb_space_link)
static 	Link					mb_space_link;
#else
#define	MB_SPACE_RGN_NUM		SYS_MALLOC_REGION_NUM
#define	MB_LINK_TOP(n)			(&mb_space_link[n])
static 	Link					mb_space_link[MB_SPACE_RGN_NUM];
#endif

/* 排他用mutex */
static MutexStruct	malloc_mutex;


KERNAPI void __sys_malloc_init(void)
{
	for ( int rgn = 0; rgn < MB_SPACE_RGN_NUM; rgn++ ) {
		link_clear(MB_LINK_TOP(rgn));
	}
	_kernel_mutex_create(&malloc_mutex);
}

KERNAPI void __sys_malloc_add_block(void* start_addr, MemSize_t size)
{
	memalloc_add_block(MB_LINK_TOP(SYS_MALLOC_REGION_DEFAULT), start_addr, size);
}


KERNAPI void* __sys_malloc_align_body(MemSize_t size, uint32_t align)
{
	for ( int rgn = 0; rgn < MB_SPACE_RGN_NUM; rgn++ ) {
		void* ret = memalloc_align(MB_LINK_TOP(rgn), size, align);
		if ( ret ) {
			return ret;
		}
	}
	return NULL;
}

KERNAPI void* __sys_malloc_body(MemSize_t size)
{
	for ( int rgn = 0; rgn < MB_SPACE_RGN_NUM; rgn++ ) {
		void* ret = memalloc(MB_LINK_TOP(rgn), size);
		if ( ret ) {
			return ret;
		}
	}
	return NULL;
}

OSAPISTUB void* __sys_malloc_align(MemSize_t size, uint32_t align)
{
	_kernel_mutex_lock(&malloc_mutex);
	void* ret = __sys_malloc_align_body(size, align);
	_kernel_mutex_unlock(&malloc_mutex);

	return ret;

}

OSAPISTUB void* __sys_malloc(MemSize_t size)
{
	_kernel_mutex_lock(&malloc_mutex);
	void* ret = __sys_malloc_body(size);
	_kernel_mutex_unlock(&malloc_mutex);

	return ret;
}

OSAPISTUB void __sys_free(void* ptr)
{
	_kernel_mutex_lock(&malloc_mutex);
	memfree(ptr);
	_kernel_mutex_unlock(&malloc_mutex);
}

#if defined(SYS_MALLOC_REGION_NUM)
KERNAPI void __sys_malloc_add_block_rgn(void* start_addr, MemSize_t size, uint32_t rgn)
{
	if ( rgn < MB_SPACE_RGN_NUM ) {
		tprintf("add rgn=%d\n", rgn);
		memalloc_add_block(MB_LINK_TOP(rgn), start_addr, size);
	}
}

KERNAPI void* __sys_malloc_align_body_rgn(MemSize_t size, uint32_t align, uint32_t rgn)
{
	void* ret = NULL;
	if ( rgn < MB_SPACE_RGN_NUM ) {
		ret = memalloc_align(MB_LINK_TOP(rgn), size, align);
	}
	return ret;
}

KERNAPI void* __sys_malloc_body_rgn(MemSize_t size, uint32_t rgn)
{
	void* ret = NULL;
	if ( rgn < MB_SPACE_RGN_NUM ) {
		ret = memalloc(MB_LINK_TOP(rgn), size);
	}
	return ret;
}

OSAPISTUB void* __sys_malloc_align_rgn(MemSize_t size, uint32_t align, uint32_t rgn)
{
	_kernel_mutex_lock(&malloc_mutex);
	void* ret = __sys_malloc_align_body_rgn(size, align, rgn);
	_kernel_mutex_unlock(&malloc_mutex);

	return ret;

}

OSAPISTUB void* __sys_malloc_rgn(MemSize_t size, uint32_t rgn)
{
	_kernel_mutex_lock(&malloc_mutex);
	void* ret = __sys_malloc_body_rgn(size, rgn);
	_kernel_mutex_unlock(&malloc_mutex);

	return ret;
}

#endif
