/*
 * board_smp.h
 *
 *  Created on: 2015/05/28
 *      Author: biwa
 */

#ifndef _BOARD_SMP_H_
#define _BOARD_SMP_H_

extern void _start(void);

static inline void smp_boot_slave_cpu(void)
{
	iowrite32(0x01C25C00+0x1a4, (uint32_t)_start);
	iowrite32(0x01C25C00+0x80, 0x00000003);
}

#endif /* _BOARD_SMP_H_ */
