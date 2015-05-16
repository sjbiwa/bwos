/*
 * spi.h
 *
 *  Created on: 2015/05/02
 *      Author: biwa
 */

#ifndef SPI_H_
#define SPI_H_

#define	SPI_FIRSTBIT_MSB				(0)
#define	SPI_FIRSTBIT_LSB				(1)
#define	SPI_ENDIAN_LITTLE				(0)
#define	SPI_ENDIAN_BIG					(1)
#define	SPI_SCPOL_LOW					(0)
#define	SPI_SCPOL_HIGH					(1)
#define	SPI_SCPH_MIDDLE					(0)
#define	SPI_SCPH_START					(1)


typedef	struct {
	uint32_t		io_addr;				/* ベースアドレス */
	uint32_t		irq;					/* IRQ番号 */
	uint32_t		clock_src;				/* クロック識別子 */
} SpiDeviceInfo;

typedef	struct {
} SpiPortConfig;

typedef	struct {
	uint32_t		baudrate;				/* ボーレート */
	uint32_t		bits;					/* データビット幅 */
	uint8_t			endian;					/* endian mode */
	uint8_t			firstbit;				/* first bit mode */
	uint16_t		scpol;					/* clock polarity */
	uint16_t		scph;					/* clock phase */
} SpiChannelConfig;

typedef	struct {
	void*			tx_buf;					/* 送信データ */
	uint32_t		tx_length;				/* 送信データ長 */
	void*			rx_buf;					/* 受信データ */
	uint32_t		rx_length;				/* 受信データ長 */
} SpiTransferParam;

extern void spi_register(SpiDeviceInfo* info, uint32_t info_num);
extern int spi_set_port_config(uint32_t port_no, SpiPortConfig* config);
extern int spi_set_channel_config(uint32_t port_no, uint32_t ch_no, SpiChannelConfig* config);
extern int spi_transfer(uint32_t port_no, uint32_t ch_no, SpiTransferParam* param);

#endif /* SPI_H_ */
