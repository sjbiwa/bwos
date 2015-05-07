/*
 * spi.h
 *
 *  Created on: 2015/05/02
 *      Author: biwa
 */

#ifndef SPI_H_
#define SPI_H_


typedef	struct {
	uint32_t		io_addr;				/* ベースアドレス */
	uint32_t		irq;					/* IRQ番号 */
	uint32_t		clock_src;				/* クロック識別子 */
} SpiDeviceInfo;

extern void spi_register(SpiDeviceInfo* info, uint32_t info_num);

#endif /* SPI_H_ */
