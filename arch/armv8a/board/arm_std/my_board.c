/*
 * my_board.c
 *
 *  Created on: 2015/12/31
 *      Author: biwa
 */

#include "kernel.h"
#include "armv8.h"
#include "armv8reg.h"

#if USE_SMP == 1
volatile uint32_t smp_boot_flag = 1;

void smp_boot_slave_cpu(void)
{
	__dsb();
	smp_boot_flag = 0;
	DC_CVAC_set(&smp_boot_flag);
	__dsb();
	__sev();
}

#endif


void board_init_task_depend(void)
{
}