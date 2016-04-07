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
static 	Link	mb_space_link;

/* 排他用mutex */
static MutexStruct	malloc_mutex;

KERNAPI void __sys_malloc_init(void)
{
	link_clear(&mb_space_link);
	_kernel_mutex_create(&malloc_mutex);
}

KERNAPI void __sys_malloc_add_block(void* start_addr, MemSize_t size)
{
	memalloc_add_block(&mb_space_link, start_addr, size);
}

KERNAPI void* __sys_malloc_align_body(MemSize_t size, uint32_t align)
{
	return memalloc_align(&mb_space_link, size, align);
}

KERNAPI void* __sys_malloc_body(MemSize_t size)
{
	return memalloc(&mb_space_link, size);
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
	memfree(&mb_space_link, ptr);
	_kernel_mutex_unlock(&malloc_mutex);
}
