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

extern void _start(void);

void smp_boot_slave_cpu(void)
{
	__dsb();
	for ( int cpuid=1; cpuid < CPU_NUM; cpuid++ ) {
		_smp_boot(0xC4000003, cpuid, _start, 0);
	}
}

#endif


void board_init_task_depend(void)
{
}
