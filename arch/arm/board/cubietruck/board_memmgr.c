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
#include "arm.h"
#include "mpcore.h"
#include "cp15reg.h"
#include "board_common.h"
#include "memmgr.h"

void board_mmgr_init(void)
{
	/* Allwinner A20 CCU/interrupt/PIO/Timer */
	mmgr_add_entry((void*)(0x01C20000), 0x1000, ATTR_DEV);
	/* UART領域 (0-7) */
	mmgr_add_entry((void*)(0x01C28000), 0x2000, ATTR_DEV);
	/* TP/PMU/CPU Configuration Register */
	mmgr_add_entry((void*)(0x01C25000), 0x3000, ATTR_DEV);
}

void board_register_normal_memory(void)
{
}

void board_init_task_depend(void)
{
}
