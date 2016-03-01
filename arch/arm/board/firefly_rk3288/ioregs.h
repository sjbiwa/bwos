/*
 * Copyright (C) 2015 sjbiwa All rights reserved.
 */

/*
 * ioregs.h
 *
 *  Created on: 2015/03/31
 *      Author: biwa
 */

#ifndef IOREGS_H_
#define IOREGS_H_

#ifdef __cplusplus
extern "C" {
#endif

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
#define	UART_GPS_BASE			(0xFF1B0000u)
#define	UART_GPS_SIZE			(0x10000u)
#define	UART_EXP_BASE			(0xFF1C0000u)
#define	UART_EXP_SIZE			(0x10000u)
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
/* UART DBG */
#define	UART_DBG_BASE			(0xFF690000u)
#define	UART_DBG_SIZE			(0x10000u)
/* TIMER */
#define	TIMER_REG_BASE			(0xFF6B0000u)
#define	TIMER_REG_SIZE			(0x10000u)
/* CCP */
#define	CCP_REG_BASE			(0xFF6C0000u)
#define	CCP_REG_SIZE			(0x10000u)
/* CCS */
#define	CCS_REG_BASE			(0xFF6E0000u)
#define	CCS_REG_SIZE			(0x10000u)
/* Internal SRAM */
#define	ISRAM_MEM_BASE			(0xFF700000u)
#define	ISRAM_MEM_SIZE			(0x20000u)
/* PMU internel mem */
#define	PMUMEM_REG_BASE			(0xFF720000u)
#define	PMUMEM_REG_SIZE			(0x1000u)
/* PMU */
#define	PMU_REG_BASE			(0xFF730000u)
#define	PMU_REG_SIZE			(0x10000u)
/* SECURE GRF */
#define	SGRF_REG_BASE			(0xFF740000u)
#define	SGRF_REG_SIZE			(0x10000u)
/* GPIO0 */
#define	GPIO0_REG_BASE			(0xFF750000u)
#define	GPIO0_REG_SIZE			(0x10000u)
/* Clock and Reset Register */
#define	CRU_REG_BASE			(0xFF760000u)
#define	CRU_REG_SIZE			(0x10000u)
/* General Register (pin mux etc) */
#define	GRF_REG_BASE			(0xFF770000u)
#define	GRF_REG_SIZE			(0x10000u)
/* GPIO1 */
#define	GPIO1_REG_BASE			(0xFF780000u)
#define	GPIO1_REG_SIZE			(0x10000u)
/* GPIO2 */
#define	GPIO2_REG_BASE			(0xFF790000u)
#define	GPIO2_REG_SIZE			(0x10000u)
/* GPIO3 */
#define	GPIO3_REG_BASE			(0xFF7A0000u)
#define	GPIO3_REG_SIZE			(0x10000u)
/* GPIO4 */
#define	GPIO4_REG_BASE			(0xFF7B0000u)
#define	GPIO4_REG_SIZE			(0x10000u)
/* GPIO5 */
#define	GPIO5_REG_BASE			(0xFF7C0000u)
#define	GPIO5_REG_SIZE			(0x10000u)
/* GPIO6 */
#define	GPIO6_REG_BASE			(0xFF7D0000u)
#define	GPIO6_REG_SIZE			(0x10000u)
/* GPIO7 */
#define	GPIO7_REG_BASE			(0xFF7E0000u)
#define	GPIO7_REG_SIZE			(0x10000u)
/* GPIO8 */
#define	GPIO8_REG_BASE			(0xFF7F0000u)
#define	GPIO8_REG_SIZE			(0x10000u)
/* WDT */
#define	WDT_REG_BASE			(0xFF800000u)
#define	WDT_REG_SIZE			(0x10000u)
/* TIMER 6-7 */
#define	TIMER67_REG_BASE		(0xFF810000u)
#define	TIMER67_REG_SIZE		(0x10000u)
/* SPDIF2 */
#define	SPDIF2_REG_BASE			(0xFF880000u)
#define	SPDIF2_REG_SIZE			(0x10000u)
/* I2SPCM */
#define	I2SPCM_REG_BASE			(0xFF890000u)
#define	I2SPCM_REG_SIZE			(0x10000u)
/* CRYPTO */
#define	CRYPTO_REG_BASE			(0xFF8A0000u)
#define	CRYPTO_REG_SIZE			(0x10000u)
/* SPDIF8 */
#define	SPDIF8_REG_BASE			(0xFF8B0000u)
#define	SPDIF8_REG_SIZE			(0x10000u)
/* IEP */
#define	IEP_REG_BASE			(0xFF900000u)
#define	IEP_REG_SIZE			(0x10000u)
/* ISP MINI */
#define	ISPMINI_REG_BASE		(0xFF910000u)
#define	ISPMINI_REG_SIZE		(0x10000u)
/* RGA */
#define	RGA_REG_BASE			(0xFF920000u)
#define	RGA_REG_SIZE			(0x10000u)
/* VOP BIG */
#define	VOPBIG_REG_BASE			(0xFF930000u)
#define	VOPBIG_REG_SIZE			(0x10000u)
/* VOP LIT */
#define	VOPLIT_REG_BASE			(0xFF940000u)
#define	VOPLIT_REG_SIZE			(0x10000u)
/* VIP */
#define	VIP_REG_BASE			(0xFF950000u)
#define	VIP_REG_SIZE			(0x10000u)
/* DSI0 */
#define	DSI0_REG_BASE			(0xFF960000u)
#define	DSI0_REG_SIZE			(0x4000u)
/* DSI1 */
#define	DSI1_REG_BASE			(0xFF964000u)
#define	DSI1_REG_SIZE			(0x4000u)
/* CSI */
#define	CSI_REG_BASE			(0xFF968000u)
#define	CSI_REG_SIZE			(0x4000u)
/* LVDS */
#define	LVDS_REG_BASE			(0xFF96C000u)
#define	LVDS_REG_SIZE			(0x4000u)
/* EDP */
#define	EDP_REG_BASE			(0xFF970000u)
#define	EDP_REG_SIZE			(0x4000u)
/* HDMI */
#define	HDMI_REG_BASE			(0xFF980000u)
#define	HDMI_REG_SIZE			(0x20000u)
/* VIDEO */
#define	VIDEO_REG_BASE			(0xFF9A0000u)
#define	VIDEO_REG_SIZE			(0x10000u)
/* HEVC */
#define	HEVC_REG_BASE			(0xFF9C0000u)
#define	HEVC_REG_SIZE			(0x10000u)
/* HOST */
#define	HOST_REG_BASE			(0xFFA20000u)
#define	HOST_REG_SIZE			(0x10000u)
/* GPU */
#define	GPU_REG_BASE			(0xFFA30000u)
#define	GPU_REG_SIZE			(0x10000u)
/* Service CORE */
#define	SRVCORE_REG_BASE		(0xFFA80000u)
#define	SRVCORE_REG_SIZE		(0x10000u)
/* Service DMA */
#define	SRVDMA_REG_BASE			(0xFFA90000u)
#define	SRVDMA_REG_SIZE			(0x10000u)
/* Service GPU */
#define	SRVGPU_REG_BASE			(0xFFAA0000u)
#define	SRVGPU_REG_SIZE			(0x10000u)
/* Service PERI */
#define	SRVPERI_REG_BASE		(0xFFAB0000u)
#define	SRVPERI_REG_SIZE		(0x10000u)
/* Service BUS */
#define	SRVBUS_REG_BASE			(0xFFAC0000u)
#define	SRVBUS_REG_SIZE			(0x10000u)
/* Service VIO */
#define	SRVVIO_REG_BASE			(0xFFAD0000u)
#define	SRVVIO_REG_SIZE			(0x10000u)
/* Service VPU */
#define	SRVVPU_REG_BASE			(0xFFAE0000u)
#define	SRVVPU_REG_SIZE			(0x10000u)
/* Service HEVC */
#define	SRVHEVC_REG_BASE		(0xFFAF0000u)
#define	SRVHEVC_REG_SIZE		(0x10000u)
/* TZPC */
#define	TZPC_REG_BASE			(0xFFB00000u)
#define	TZPC_REG_SIZE			(0x10000u)
/* eFuse-1024bits */
#define	EFUSE1024_REG_BASE		(0xFFB10000u)
#define	EFUSE1024_REG_SIZE		(0x10000u)
/* Security DMAC_BUS */
#define	SECDMAC_REG_BASE		(0xFFB20000u)
#define	SECDMAC_REG_SIZE		(0x10000u)
/* eFuse-256bits */
#define	EFUSE256_REG_BASE		(0xFFB40000u)
#define	EFUSE256_REG_SIZE		(0x10000u)
/* A17 Debug */
#define	A17DBG_REG_BASE			(0xFFB80000u)
#define	A17DBG_REG_SIZE			(0x80000u)
/* Core GIC */
#define	COREGIC_REG_BASE		(0xFFC00000u)
#define	COREGIC_REG_SIZE		(0x80000u)
/* A17 Debug */
#define	A17DBG_REG_BASE			(0xFFB80000u)
#define	A17DBG_REG_SIZE			(0x80000u)
/* CORE SLV */
#define	CORESLV_REG_BASE		(0xFFC80000u)
#define	CORESLV_REG_SIZE		(0x80000u)
/* CORE AXI bus */
#define	COREAXI_REG_BASE		(0xFFD00000u)
#define	COREAXI_REG_SIZE		(0x100000u)

#ifdef __cplusplus
}
#endif

#endif /* IOREGS_H_ */
