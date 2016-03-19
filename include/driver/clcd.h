/*
 * Copyright (C) 2016 sjbiwa All rights reserved.
 */

/*
 * clcd.h
 *
 *  Created on: 2016/03/18
 *      Author: biwa
 */

#ifndef _CLCD_H_
#define _CLCD_H_

#include "bwos.h"

typedef	struct {
	uint32_t		io_addr;				/* ベースアドレス */
	uint32_t		irq;					/* IRQ番号 */
	uint32_t		clock_src;				/* クロック識別子 */
} ClcdDeviceInfo;

extern void clcd_register(ClcdDeviceInfo* info, uint32_t info_num);

#endif /* _CLCD_H_ */
