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

void smp_boot_slave_cpu(uint32_t cluster, uint32_t slave_cpu_num)
{
	slave_cpu_num = CPU_PER_CLUSTER;
	for ( uint32_t slave_cpu = 1; slave_cpu < slave_cpu_num; slave_cpu++ ) {
		_smp_boot(0xC4000003, (cluster << 8) | slave_cpu, _start, 0);
	}
}

void smp_boot_cluster_master_cpu(uint32_t cluster)
{
	_smp_boot(0xC4000003, (cluster << 8) | 0, _start, 0);
}

#endif


void board_init_task_depend(void)
{
}
