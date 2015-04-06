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
#include "ioregs.h"

void board_mmgr_init(void)
{
	/* CRU(Clock and Reset)領域 */
	mmgr_add_entry((void*)(CRU_REG_BASE), CRU_REG_SIZE, ATTR_DEV);

	/* GRF(General Register)領域 */
	mmgr_add_entry((void*)(GRF_REG_BASE), GRF_REG_SIZE, ATTR_DEV);

	/* UART領域 */
	mmgr_add_entry((void*)(UART_BT_BASE), UART_BT_SIZE, ATTR_DEV);
	mmgr_add_entry((void*)(UART_BB_BASE), UART_BB_SIZE, ATTR_DEV);
	mmgr_add_entry((void*)(UART_DBG_BASE), UART_DBG_SIZE, ATTR_DEV);
	mmgr_add_entry((void*)(UART_GPS_BASE), UART_GPS_SIZE, ATTR_DEV);
	mmgr_add_entry((void*)(UART_EXP_BASE), UART_EXP_SIZE, ATTR_DEV);
}
