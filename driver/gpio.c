/*
 * Copyright (C) 2015 syujibiwa All rights reserved.
 */

/*
 * gpio.c
 *
 *  Created on: 2015/04/19
 *      Author: biwa
 */
#include "bwos.h"
#include "driver/gpio.h"
#include "driver/clock.h"


/* GOIOレジスタ定義 */
#define	GPIO_SWPORTA_DR 	(0x0000u) 	/* Port A data register */
#define	GPIO_SWPORTA_DDR 	(0x0004u) 	/* Port A data direction register */
#define	GPIO_INTEN 			(0x0030u) 	/* Interrupt enable register */
#define	GPIO_INTMASK 		(0x0034u) 	/* Interrupt mask register */
#define	GPIO_INTTYPE_LEVEL 	(0x0038u) 	/* Interrupt level register */
#define	GPIO_INT_POLARITY 	(0x003cu) 	/* Interrupt polarity register */
#define	GPIO_INT_STATUS 	(0x0040u) 	/* Interrupt status of port A */
#define	GPIO_INT_RAWSTATUS 	(0x0044u) 	/* Raw Interrupt status of port A */
#define	GPIO_DEBOUNCE 		(0x0048u) 	/* Debounce enable register */
#define	GPIO_PORTA_EOI 		(0x004cu) 	/* Port A clear interrupt register */
#define	GPIO_EXT_PORTA 		(0x0050u) 	/* Port A external port register */
#define	GPIO_LS_SYNC 		(0x0060u) 	/* Level_sensitive synchronization enable register */

typedef	struct {
	GpioDeviceInfo*	dev;					/* デバイス情報 */
} GpioObject;

static GpioObject*		gpio_obj_tbl = 0;
static uint32_t			gpio_obj_num = 0;

static GpioObject* gpio_get_object(uint32_t portno)
{
	return &gpio_obj_tbl[portno];
}

static uint32_t gpio_get_addr(uint32_t portno)
{
	return gpio_get_object(portno)->dev->io_addr;
}

void gpio_register(GpioDeviceInfo* info, uint32_t info_num)
{
	int ix;
	gpio_obj_num = info_num;

	gpio_obj_tbl = sys_malloc(sizeof(GpioObject) * info_num);
	for ( ix=0; ix < info_num; ix++ ) {
		gpio_obj_tbl[ix].dev = &info[ix];
	}
}

void gpio_set_direction(uint32_t port_no, uint32_t dir, uint32_t mask)
{
	if ( port_no < gpio_obj_num ) {
		uint32_t port = gpio_get_addr(port_no) + GPIO_SWPORTA_DDR;
		iowrite32(port, (ioread32(port) & ~mask) | dir);
	}
}

void gpio_set_value(uint32_t port_no, uint32_t value, uint32_t mask)
{
	if ( port_no < gpio_obj_num ) {
		uint32_t port = gpio_get_addr(port_no) + GPIO_SWPORTA_DR;
		iowrite32(port, (ioread32(port) & ~mask) | value);
	}
}

uint32_t gpio_get_value(uint32_t port_no)
{
	uint32_t ret = 0;
	if ( port_no < gpio_obj_num ) {
		uint32_t port = gpio_get_addr(port_no) + GPIO_SWPORTA_DR;
		ret = ioread32(port);
	}
	return ret;
}

void gpio_set_bit(uint32_t port_no, uint32_t bit, uint32_t value)
{
	if ( port_no < gpio_obj_num ) {
		uint32_t port = gpio_get_addr(port_no) + GPIO_SWPORTA_DR;
		iowrite32(port, (ioread32(port) & ~(1u<<bit)) | ((value?1u:0u)<<bit));
	}
}

uint32_t gpio_get_bit(uint32_t port_no, uint32_t bit)
{
	uint32_t ret = 0;
	if ( port_no < gpio_obj_num ) {
		uint32_t port = gpio_get_addr(port_no) + GPIO_SWPORTA_DR;
		ret = (ioread32(port) & (1u<<bit)) ? 1 : 0;
	}
	return ret;
}
