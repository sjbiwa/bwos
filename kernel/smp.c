/*
 * smp.c
 *
 *  Created on: 2015/05/26
 *      Author: biwa
 */

#if USE_SMP == 1

#include "kernel/smp.h"

void smp_init(void)
{
	arch_smp_init();
}
#endif
