/*
 * Copyright (C) 2015 sjbiwa All rights reserved.
 */

/*
 * memmgr.c
 *
 *  Created on: 2014/12/07
 *      Author: biwa
 */

#include "kernel.h"
#include "armv8.h"
#include "my_board.h"
#include "memmgr.h"

void board_mmgr_init(void)
{
	mmgr_add_entry((void*)(SYSTEM_BASE), (uint64_t)(0x10000LL), ATTR_DEV);
	mmgr_add_entry((void*)(UART_BASE), (uint64_t)(0x10000LL), ATTR_DEV);
	mmgr_add_entry((void*)(0x0880000000uLL), (uint64_t)(0x180000000uLL), ATTR_DATA);
}

void board_register_normal_memory(void)
{
	sys_malloc_add_block((void*)(0x0880000000uLL), (MemSize_t)(0x180000000uLL));
}
