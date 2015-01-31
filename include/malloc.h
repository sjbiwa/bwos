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
extern void sys_malloc_add_block(void* addr, uint32_t size);
extern void* sys_malloc_body(uint32_t size);
extern void* sys_malloc_align_body(uint32_t size, uint32_t align);

#endif /* MALLOC_H_ */
