/*
 * ioregs.h
 *
 *  Created on: 2015/03/31
 *      Author: biwa
 */

#ifndef IOREGS_H_
#define IOREGS_H_

/* GPS */
#define	GPS_REG_BASE			(0xFF000000u)
#define	GPS_REG_SIZE			(0x80000u)
/* HSADC */
#define	HSADC_REG_BASE			(0xFF080000u)
#define	HSADC_REG_SIZE			(0x40000u)
/* SD/MMC */
#define	SDMMC_REG_BASE			(0xFF0C0000u)
#define	SDMMC_REG_SIZE			(0x10000u)
/* SDIO0 */
#define	SDIO0_REG_BASE			(0xFF0D0000u)
#define	SDIO0_REG_SIZE			(0x10000u)
/* SDIO1 */
#define	SDIO1_REG_BASE			(0xFF0E0000u)
#define	SDIO1_REG_SIZE			(0x10000u)
/* eMMC */
#define	EMMC_REG_BASE			(0xFF0F0000u)
#define	EMMC_REG_SIZE			(0x10000u)
/* SAR ADC */
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
/* DMAC PERI */
#define	DMACPERI_REG_BASE		(0xFF250000u)
#define	DMACPERI_REG_SIZE		(0x10000u)
/* PS2C */
#define	PS2C_REG_BASE			(0xFF260000u)
#define	PS2C_REG_SIZE			(0x10000u)
/* TSADC */
#define	TSADC_REG_BASE			(0xFF280000u)
#define	TSADC_REG_SIZE			(0x10000u)
/* GMAC */
#define	GMAC_REG_BASE			(0xFF290000u)
#define	GMAC_REG_SIZE			(0x10000u)
/* PERI MMU */
#define	PERIMMU_REG_BASE		(0xFF2A0000u)
#define	PERIMMU_REG_SIZE		(0x10000u)
/* PERI AXI */
#define	PERIAXI_REG_BASE		(0xFF300000u)
#define	PERIAXI_REG_SIZE		(0x100000u)
/* NANDC0 */
#define	NANDC0_REG_BASE			(0xFF400000u)
#define	NANDC0_REG_SIZE			(0x10000u)
/* NANDC1 */
#define	NANDC1_REG_BASE			(0xFF410000u)
#define	NANDC1_REG_SIZE			(0x10000u)
/* TSP */
#define	TSP_REG_BASE			(0xFF420000u)
#define	TSP_REG_SIZE			(0x10000u)
/* USB EHCI */
#define	EHCI_REG_BASE			(0xFF500000u)
#define	EHCI_REG_SIZE			(0x20000u)
/* USB OHCI */
#define	OHCI_REG_BASE			(0xFF520000u)
#define	OHCI_REG_SIZE			(0x20000u)
/* USB HOST */
#define	USBH_REG_BASE			(0xFF540000u)
#define	USBH_REG_SIZE			(0x40000u)
/* USB OTG */
#define	OTG_REG_BASE			(0xFF580000u)
#define	OTG_REG_SIZE			(0x40000u)
/* HSIC */
#define	HSIC_REG_BASE			(0xFF5C0000u)
#define	HSIC_REG_SIZE			(0x40000u)
/* DMACBUS */
#define	DMACBUS_REG_BASE		(0xFF600000u)
#define	DMACBUS_REG_SIZE		(0x10000u)
/* DDR_PCTL0 */
#define	DDRPCTL0_REG_BASE		(0xFF610000u)
#define	DDRPCTL0_REG_SIZE		(0x10000u)
/* DDR_PUBL0 */
#define	DDRPUBL0_REG_BASE		(0xFF620000u)
#define	DDRPUBL0_REG_SIZE		(0x10000u)
/* DDR_PCTL1 */
#define	DDRPCTL1_REG_BASE		(0xFF630000u)
#define	DDRPCTL1_REG_SIZE		(0x10000u)
/* DDR_PUBL1 */
#define	DDRPUBL1_REG_BASE		(0xFF640000u)
#define	DDRPUBL1_REG_SIZE		(0x10000u)
/* I2C_PMU */
#define	I2CPMU_REG_BASE			(0xFF650000u)
#define	I2CPMU_REG_SIZE			(0x10000u)
/* I2C_AUDIO */
#define	I2CAUDIO_REG_BASE		(0xFF660000u)
#define	I2CAUDIO_REG_SIZE		(0x10000u)
/* DW_PWM */
#define	DWPWM_REG_BASE			(0xFF670000u)
#define	DWPWM_REG_SIZE			(0x10000u)
/* RK_PWM */
#define	RKPWM_REG_BASE			(0xFF680000u)
#define	RKPWM_REG_SIZE			(0x10000u)
/* TIMER */
#define	TIMER_REG_BASE			(0xFF6B0000u)
#define	TIMER_REG_SIZE			(0x10000u)
/* CCP */
#define	CCP_REG_BASE			(0xFF6C0000u)
#define	CCP_REG_SIZE			(0x10000u)
/* CCS */
#define	CCS_REG_BASE			(0xFF6E0000u)
#define	CCS_REG_SIZE			(0x10000u)

#endif /* IOREGS_H_ */
