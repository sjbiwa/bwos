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

	/* GPIO領域 */
	mmgr_add_entry((void*)(GPIO0_REG_BASE), GPIO0_REG_SIZE, ATTR_DEV);
	mmgr_add_entry((void*)(GPIO1_REG_BASE), GPIO1_REG_SIZE, ATTR_DEV);
	mmgr_add_entry((void*)(GPIO2_REG_BASE), GPIO2_REG_SIZE, ATTR_DEV);
	mmgr_add_entry((void*)(GPIO3_REG_BASE), GPIO3_REG_SIZE, ATTR_DEV);
	mmgr_add_entry((void*)(GPIO4_REG_BASE), GPIO4_REG_SIZE, ATTR_DEV);
	mmgr_add_entry((void*)(GPIO5_REG_BASE), GPIO5_REG_SIZE, ATTR_DEV);
	mmgr_add_entry((void*)(GPIO6_REG_BASE), GPIO6_REG_SIZE, ATTR_DEV);
	mmgr_add_entry((void*)(GPIO7_REG_BASE), GPIO7_REG_SIZE, ATTR_DEV);
	mmgr_add_entry((void*)(GPIO8_REG_BASE), GPIO8_REG_SIZE, ATTR_DEV);

	/* UART領域 */
	mmgr_add_entry((void*)(UART_BT_BASE), UART_BT_SIZE, ATTR_DEV);
	mmgr_add_entry((void*)(UART_BB_BASE), UART_BB_SIZE, ATTR_DEV);
	mmgr_add_entry((void*)(UART_DBG_BASE), UART_DBG_SIZE, ATTR_DEV);
	mmgr_add_entry((void*)(UART_GPS_BASE), UART_GPS_SIZE, ATTR_DEV);
	mmgr_add_entry((void*)(UART_EXP_BASE), UART_EXP_SIZE, ATTR_DEV);

	/* LVDS */
	mmgr_add_entry((void*)(LVDS_REG_BASE), LVDS_REG_SIZE, ATTR_DEV);
	/* VOP BIG */
	mmgr_add_entry((void*)(VOPBIG_REG_BASE), VOPBIG_REG_SIZE, ATTR_DEV);
	/* VOP LIT */
	mmgr_add_entry((void*)(VOPLIT_REG_BASE), VOPLIT_REG_SIZE, ATTR_DEV);

	/* PMU */
	mmgr_add_entry((void*)(PMU_REG_BASE), PMU_REG_SIZE, ATTR_DEV);

	/* FRAME BUFFER */
	mmgr_add_entry((void*)(FRAME_BUFFER_ADDR), FRAME_BUFFER_SIZE, ATTR_DEV);

	/* SPI (SPI1 not use) */
	mmgr_add_entry((void*)(SPI0_REG_BASE), SPI0_REG_SIZE, ATTR_DEV);
	mmgr_add_entry((void*)(SPI2_REG_BASE), SPI2_REG_SIZE, ATTR_DEV);

	/* I2C */
	mmgr_add_entry((void*)(I2CPMU_REG_BASE), 	I2CPMU_REG_SIZE,	ATTR_DEV);
	mmgr_add_entry((void*)(I2CAUDIO_REG_BASE), 	I2CAUDIO_REG_SIZE,	ATTR_DEV);
	mmgr_add_entry((void*)(I2CSENSOR_REG_BASE), I2CSENSOR_REG_SIZE,	ATTR_DEV);
	mmgr_add_entry((void*)(I2CCAM_REG_BASE), 	I2CCAM_REG_SIZE,	ATTR_DEV);
	mmgr_add_entry((void*)(I2CTP_REG_BASE), 	I2CTP_REG_SIZE,		ATTR_DEV);
	mmgr_add_entry((void*)(I2CHDMI_REG_BASE), 	I2CHDMI_REG_SIZE,	ATTR_DEV);

	/* Internal SRAM */
	mmgr_add_entry((void*)(ISRAM_MEM_BASE), 	ISRAM_MEM_SIZE,	ATTR_DEV);
}
