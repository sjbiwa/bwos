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
	/* CRU(Clock and Reset)領域 */
	mmgr_add_entry((void*)(0xFF760000u), 0x10000, ATTR_DEV);

	/* GRF(General Register)領域 */
	mmgr_add_entry((void*)(0xFF770000u), 0x10000, ATTR_DEV);

	/* UART領域 */
	mmgr_add_entry((void*)(0xFF180000u), 0x10000, ATTR_DEV);
	mmgr_add_entry((void*)(0xFF190000u), 0x10000, ATTR_DEV);
	mmgr_add_entry((void*)(0xFF1B0000u), 0x10000, ATTR_DEV);
	mmgr_add_entry((void*)(0xFF1C0000u), 0x10000, ATTR_DEV);
	mmgr_add_entry((void*)(0xFF690000u), 0x10000, ATTR_DEV);
}
