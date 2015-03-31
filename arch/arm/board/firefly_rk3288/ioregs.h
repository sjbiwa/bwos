/*
 * ioregs.h
 *
 *  Created on: 2015/03/31
 *      Author: biwa
 */

#ifndef IOREGS_H_
#define IOREGS_H_

#define	GPS_REG_BASE			(0xFF000000u)
#define	GPS_REG_SIZE			(0x80000u)

#define	HSADC_REG_BASE			(0xFF080000u)
#define	HSADC_REG_SIZE			(0x40000u)

#define	SDMMC_REG_BASE			(0xFF0C0000u)
#define	SDMMC_REG_SIZE			(0x10000u)

#define	SDIO0_REG_BASE			(0xFF0D0000u)
#define	SDIO0_REG_SIZE			(0x10000u)

#define	SDIO0_REG_BASE			(0xFF0E0000u)
#define	SDIO0_REG_SIZE			(0x10000u)

#define	EMMC_REG_BASE			(0xFF0F0000u)
#define	EMMC_REG_SIZE			(0x10000u)

#define	SARADC_REG_BASE			(0xFF100000u)
#define	SARADC_REG_SIZE			(0x10000u)

/* SPI */
#define	SPI0_REG_BASE			(0xFF110000u)
#define	SPI0_REG_SIZE			(0x10000u)
#define	SPI1_REG_BASE			(0xFF120000u)
#define	SPI1_REG_SIZE			(0x10000u)
#define	SPI2_REG_BASE			(0xFF130000u)
#define	SPI2_REG_SIZE			(0x10000u)

/* I2C */
#define	I2CSENSOR_REG_BASE		(0xFF140000u)
#define	I2CSENSOR_REG_SIZE		(0x10000u)
#define	I2CCAM_REG_BASE			(0xFF150000u)
#define	I2CCAM_REG_SIZE			(0x10000u)
#define	I2CTP_REG_BASE			(0xFF160000u)
#define	I2CTP_REG_SIZE			(0x10000u)
#define	I2CHDMI_REG_BASE		(0xFF170000u)
#define	I2CHDMI_REG_SIZE		(0x10000u)

/* UART */
#define	UART_BT_BASE			(0xFF180000u)
#define	UART_BT_SIZE			(0x10000u)
#define	UART_BB_BASE			(0xFF190000u)
#define	UART_BB_SIZE			(0x10000u)
#define	UART_DBG_BASE			(0xFF690000u)
#define	UART_DBG_SIZE			(0x10000u)
#define	UART_GPS_BASE			(0xFF1B0000u)
#define	UART_GPS_SIZE			(0x10000u)
#define	UART_EXP_BASE			(0xFF1C0000u)
#define	UART_EXP_SIZE			(0x10000u)

/* Clock and Reset Register */
#define	CRU_REG_BASE			(0xFF760000u)
#define	CRU_REG_SIZE			(0x10000u)

/* General Register (pin mux etc) */
#define	GRF_REG_BASE			(0xFF770000u)
#define	GRF_REG_SIZE			(0x10000u)



#endif /* IOREGS_H_ */
