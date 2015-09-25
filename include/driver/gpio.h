/*
 * Copyright (C) 2015 sjbiwa All rights reserved.
 */

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
	uint32_t		io_addr;				/* ベースアドレス */
} GpioDeviceInfo;

extern void gpio_register(GpioDeviceInfo* info, uint32_t info_num);
extern void gpio_set_direction(uint32_t port_no, uint32_t dir, uint32_t mask);
extern void gpio_set_value(uint32_t port_no, uint32_t value, uint32_t mask);
extern uint32_t gpio_get_value(uint32_t port_no);
extern void gpio_set_bit(uint32_t port_no, uint32_t bit, uint32_t value);
extern uint32_t gpio_get_bit(uint32_t port_no, uint32_t bit);

#endif /* UART_H_ */
