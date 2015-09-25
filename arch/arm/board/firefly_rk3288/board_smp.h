/*
 * Copyright (C) 2015 sjbiwa All rights reserved.
 */

/*
 * board_smp.h
 *
 *  Created on: 2015/05/28
 *      Author: biwa
 */

#ifndef _BOARD_SMP_H_
#define _BOARD_SMP_H_

#include "common.h"
#include "irqdefs.h"
#include "ioregs.h"
#include "cruregs.h"
#include "grfregs.h"

extern void _start(void);

static inline void smp_boot_slave_cpu(void)
{
	__dsb();
	iowrite32(ISRAM_MEM_BASE+8, (uint32_t)_start);
	iowrite32(ISRAM_MEM_BASE+4, 0xDEADBEAF);
	__dsb();
	__sev();
}

#endif /* _BOARD_SMP_H_ */
