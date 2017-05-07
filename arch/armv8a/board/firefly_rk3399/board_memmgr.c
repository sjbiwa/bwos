/*
 * Copyright (C) 2016 sjbiwa All rights reserved.
 */

/*
 * board_memmgr.c
 *
 *  Created on: 2016/03/13
 *      Author: biwa
 */

#include "kernel.h"
#include "armv8.h"
#include "my_board.h"
#include "memmgr.h"

void board_mmgr_init(void)
{
	mmgr_add_entry((void*)(UART_BASE), (uint64_t)(0x10000LL), ATTR_DEV);
}

void board_register_normal_memory(void)
{
}
