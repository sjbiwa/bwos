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

#ifdef __cplusplus
extern "C" {
#endif

extern void _start(void);

static inline void smp_boot_slave_cpu(void)
{
	__dsb();
	iowrite32(0x01C25C00+0x1a4, (uint32_t)_start);
	ioclr32(0x01C25C00+0x184, 0x02u);
	iowrite32(0x01C25C00+0x80, 0x00000003);
	__dsb();
}

#ifdef __cplusplus
}
#endif

#endif /* _BOARD_SMP_H_ */
