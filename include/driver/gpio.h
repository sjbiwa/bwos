/*
 * gpio.h
 *
 *  Created on: 2015/04/19
 *      Author: biwa
 */

#ifndef GPIO_H_
#define GPIO_H_

#include "bwos.h"

typedef	struct {
	uint8_t*		io_addr;				/* ベースアドレス */
} GpioDeviceInfo;

extern void gpio_register(GpioDeviceInfo* info, uint32_t info_num);

#endif /* UART_H_ */
