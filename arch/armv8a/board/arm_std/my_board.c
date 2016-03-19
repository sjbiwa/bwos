/*
 * my_board.c
 *
 *  Created on: 2015/12/31
 *      Author: biwa
 */

#include "kernel.h"
#include "armv8.h"
#include "armv8reg.h"
#include "driver/clock.h"
#include "driver/clcd.h"

#if USE_SMP == 1
volatile uint32_t smp_boot_flag = 1;

void smp_boot_slave_cpu(uint32_t cluster, uint32_t slave_cpu_num)
{
	__dsb();
	smp_boot_flag = 0;
	DC_CVAC_set(&smp_boot_flag);
	__dsb();
	__sev();
}

void smp_boot_cluster_master_cpu(uint32_t cluster)
{
}

#endif


#define	CLOCK_UART0			0
#define	CLOCK_UART1			1
#define	CLOCK_CLCD			2

static ClockRegisterParam clock_params[] = {
	[CLOCK_UART0] 	= { 24000000 },
	[CLOCK_UART1] 	= { 24000000 },
	[CLOCK_CLCD] 	= { 25175000 },
};

static ClcdDeviceInfo clcd_info[] = {
		{ CLCD_REG_BASE,  46,  CLOCK_CLCD},	/* CLCD  */
};

void board_init_task_depend(void)
{
	clock_register(clock_params, arrayof(clock_params));

	clcd_register(clcd_info, arrayof(clcd_info));
}
