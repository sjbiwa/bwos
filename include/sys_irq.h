/*
 * Copyright (C) 2015 sjbiwa All rights reserved.
 */

/*
 * sys_irq.h
 *
 *  Created on: 2015/01/31
 *      Author: biwa
 */

#include <stdint.h>

#ifndef SYS_IRQ_H_
#define SYS_IRQ_H_

typedef	void (*IRQ_HANDLER)(uint32_t irqno, void* info);

#endif /* SYS_IRQ_H_ */
