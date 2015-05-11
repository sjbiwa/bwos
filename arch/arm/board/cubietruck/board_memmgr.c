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
#include "my_board.h"
#include "memmgr.h"

void board_mmgr_init(void)
{
	/* Allwinner A20 CCU/interrupt/PIO/Timer */
	mmgr_add_entry((void*)(0x01C20000), 0x1000, ATTR_DEV);
	/* UART領域 */
	mmgr_add_entry((void*)(0x01C28000), 0x1000, ATTR_DEV);

}

void init_task_board_depend(void)
{
}
