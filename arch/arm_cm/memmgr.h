/*
 * memmgr.c
 *
 *  Created on: 2015/07/01
 *      Author: biwa
 */

#include "kernel.h"
#include "arm.h"

extern void mmgr_add_entry(void* addr, MemSize_t size, uint32_t attr);
extern void board_mmgr_init(void);
