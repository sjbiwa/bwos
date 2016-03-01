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

/* クラスタマスタコアの起動 */
void smp_boot_cluster_master_cpu(void)
{
	__dsb();
	uint32_t cluster = (CPU_NUM-1) / CPU_PER_CLUSTER; /* 起動すべきクラスタマスタ数 */
	for ( uint32_t mpid = 0x100; 0 < cluster; mpid += 0x100, cluster-- ) {
		tprintf("BOOT:%08X\n", mpid);
		_smp_boot(0xC4000003, mpid, _start, 0);
	}
}

/* 自クラスタ内スレーブコア起動 */
void smp_boot_slave_cpu(void)
{
	__dsb();
	uint32_t cluster = (MPIDR_EL1_get() >> 8) & 0xff; /* 自身のクラスタ番号 */
	uint32_t first_core = cluster * CPU_PER_CLUSTER + 1; /* 最初のスレーブコア番号 */
	uint32_t end_core = first_core + CPU_PER_CLUSTER - 2; /* 最後のスレーブコア番号 */
	if ( CPU_NUM <= end_core ) {
		end_core = CPU_NUM - 1;
	}
	first_core -= cluster * CPU_PER_CLUSTER;
	end_core -= cluster * CPU_PER_CLUSTER;
	for ( ; first_core <= end_core; first_core++ ) {
		tprintf("BOOT:%08X\n", (cluster << 8) | first_core);
		_smp_boot(0xC4000003, (cluster << 8) | first_core, _start, 0);
	}
}

#endif


void board_init_task_depend(void)
{
}
