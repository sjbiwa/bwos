/*
 * Copyright (C) 2015 sjbiwa All rights reserved.
 */

/*
 * malloc.h
 *
 *  Created on: 2014/11/19
 *      Author: biwa
 */

#ifndef MALLOC_H_
#define MALLOC_H_

#include <stdint.h>
#include "common.h"

extern void sys_malloc_init(void);
extern void sys_malloc_add_block(void* addr, MemSize_t size);
extern void* sys_malloc_body(MemSize_t size);
extern void* sys_malloc_align_body(MemSize_t size, uint32_t align);

extern void st_malloc_init(void* mem_addr, MemSize_t mem_size);
extern void* st_malloc_align(MemSize_t alloc_size, uint32_t align);
extern void st_malloc_normalize(void);

#endif /* MALLOC_H_ */
