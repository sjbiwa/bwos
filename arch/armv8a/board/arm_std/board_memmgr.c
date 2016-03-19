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
	mmgr_add_entry((void*)(SYSTEM_REG_BASE), (uint64_t)(SYSTEM_REG_SIZE), ATTR_DEV);
	mmgr_add_entry((void*)(UART_REG_BASE), (uint64_t)(UART_REG_SIZE), ATTR_DEV);
	mmgr_add_entry((void*)(CLCD_REG_BASE), (uint64_t)(CLCD_REG_SIZE), ATTR_DEV);
	mmgr_add_entry((void*)(FRAME_BUFFER_BASE), (uint64_t)(FRAME_BUFFER_SIZE), ATTR_NONCACHE);
	mmgr_add_entry((void*)(SDMMC_MAP_BASE), (uint64_t)(SDMMC_MAP_SIZE), ATTR_DATA);
}

void board_register_normal_memory(void)
{
	//__sys_malloc_add_block((void*)(0x0880000000uLL), (MemSize_t)(0x180000000uLL));
}
