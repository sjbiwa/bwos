/*
 * Copyright (C) 2015 sjbiwa All rights reserved.
 */

/*
 * boards.c
 *
 *  Created on: 2015/03/31
 *      Author: biwa
 */

#include "bwos.h"
#include "driver/clock.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "driver/spi.h"
#include "driver/i2c.h"
#include "driver/sdmmc.h"
#include "irqdefs.h"
#include "ioregs.h"
#include "cruregs.h"
#include "grfregs.h"

#define	CLOCK_UART0			0
#define	CLOCK_UART1			1
#define	CLOCK_UART2			2
#define	CLOCK_UART3			3
#define	CLOCK_UART4			4
#define	CLOCK_SPI0			5
#define	CLOCK_SPI1			6
#define	CLOCK_SPI2			7
#define	CLOCK_I2C0			8
#define	CLOCK_I2C1			9
#define	CLOCK_I2C2			10
#define	CLOCK_I2C3			11
#define	CLOCK_I2C4			12
#define	CLOCK_I2C5			13
#define	CLOCK_SDMMC0		14


static const uint32_t	cru = CRU_REG_BASE;
static const uint32_t	grf = GRF_REG_BASE;
static const uint32_t	lvds = LVDS_REG_BASE;
static const uint32_t	pmu = PMU_REG_BASE;

static ClockRegisterParam clock_params[] = {
	[CLOCK_UART0] 	= { 24000000 },
	[CLOCK_UART1] 	= { 24000000 },
	[CLOCK_UART2] 	= { 24000000 },
	[CLOCK_UART3] 	= { 24000000 },
	[CLOCK_UART4] 	= { 24000000 },
	[CLOCK_SPI0]	= { 48000000 },
	[CLOCK_SPI1]	= {        0 },	/* not used */
	[CLOCK_SPI2]	= { 48000000 },
	[CLOCK_I2C0]	= { 74000000 },
	[CLOCK_I2C1]	= { 74000000 },
	[CLOCK_I2C2]	= { 74000000 },
	[CLOCK_I2C3]	= { 74000000 },
	[CLOCK_I2C4]	= { 74000000 },
	[CLOCK_I2C5]	= { 74000000 },
	[CLOCK_SDMMC0]	= { 48000000 },
};

static UartDeviceInfo uart_info[] = {
		{ UART_BT_BASE,  IRQ_UART_BT,  CLOCK_UART0},	/* UART_BT  */
		{ UART_BB_BASE,  IRQ_UART_BB,  CLOCK_UART1},	/* UART_BB  */
		{ UART_DBG_BASE, IRQ_UART_DBG, CLOCK_UART2},	/* UART_DBG */
		{ UART_GPS_BASE, IRQ_UART_GPS, CLOCK_UART3},	/* UART_GPS */
		{ UART_EXP_BASE, IRQ_UART_EXP, CLOCK_UART4},	/* UART_EXP */
};

static GpioDeviceInfo gpio_info[] = {
		{ GPIO0_REG_BASE },								/* GPIO0 */
		{ GPIO1_REG_BASE },								/* GPIO1 */
		{ GPIO2_REG_BASE },								/* GPIO2 */
		{ GPIO3_REG_BASE },								/* GPIO3 */
		{ GPIO4_REG_BASE },								/* GPIO4 */
		{ GPIO5_REG_BASE },								/* GPIO5 */
		{ GPIO6_REG_BASE },								/* GPIO6 */
		{ GPIO7_REG_BASE },								/* GPIO7 */
		{ GPIO8_REG_BASE },								/* GPIO8 */
};

static SpiDeviceInfo spi_info[] = {
		{ SPI0_REG_BASE,  IRQ_SPI0,  CLOCK_SPI0},	/* SPI0  */
		{ SPI1_REG_BASE,  IRQ_SPI1,  CLOCK_SPI1},	/* SPI1 (not used)  */
		{ SPI2_REG_BASE,  IRQ_SPI2,  CLOCK_SPI2},	/* SPI2  */
};

static I2cDeviceInfo i2c_info[] = {
		{ I2CPMU_REG_BASE,		IRQ_I2C_PMU,	CLOCK_I2C0},	/* I2C PMU  */
		{ I2CAUDIO_REG_BASE,	IRQ_I2C_AUDIO,	CLOCK_I2C1},	/* I2C AUDIO  */
		{ I2CSENSOR_REG_BASE,	IRQ_I2C_SENSOR, CLOCK_I2C2},	/* I2C SENSOR  */
		{ I2CCAM_REG_BASE,  	IRQ_I2C_CAM,	CLOCK_I2C3},	/* I2C CAM  */
		{ I2CTP_REG_BASE,		IRQ_I2C_TP,		CLOCK_I2C4},	/* I2C TP  */
		{ I2CHDMI_REG_BASE,		IRQ_I2C_HDMI,	CLOCK_I2C5},	/* I2C HDMI  */
};

static SdmmcDeviceInfo sdmmc_info[] = {
		{ SDMMC_REG_BASE,		IRQ_SDMMC,		CLOCK_SDMMC0},	/* SDMMC0  */
};


void board_init_task_depend(void)
{
	/****************************************/
	/* firefly3288: 						*/
	/*  arm pll    = 600000000HZ			*/
	/*  periph pll = 297000000HZ			*/
	/*  ddr pll    = 200000000HZ			*/
	/*  codec pll  = 384000000HZ			*/
	/****************************************/

	/************************/
	/* clock configure		*/
	/************************/
	iowrite32(cru+CRU_CLKSEL13_CON, 0xffff0200); /* UART_BT / 24MHz */
	iowrite32(cru+CRU_CLKSEL14_CON, 0xffff0200); /* UART_BB / 24MHz */
	iowrite32(cru+CRU_CLKSEL16_CON, 0xffff0200); /* UART_GPS / 24MHz */
	iowrite32(cru+CRU_CLKSEL3_CON, 0xffff0200); /* UART_EXP / 24MHz */
	iowrite32(cru+CRU_CLKSEL27_CON, 0xffff0400); /* LCDC0 clock */
	iowrite32(cru+CRU_CLKSEL25_CON, 0xffff0707); /* SPI0/SPI1 clock(codec pll / 8 = 48MHz) */
	iowrite32(cru+CRU_CLKSEL39_CON, 0x00ff0007); /* SPI02 clock(codec pll / 8 = 48MHz) */
	iowrite32(cru+CRU_CLKSEL11_CON, 0x00ff0007); /* SDMMC0 clock(codec pll / 8 = 48MHz) */

	clock_register(clock_params, arrayof(clock_params));

	/************************/
	/* pin configure		*/
	/************************/
	/* for UART */
	iowrite32(grf+GRF_GPIO4C_IOMUX, 0x00550055); /* UART_BT rts/cts/in/out */
	iowrite32(grf+GRF_GPIO5B_IOMUX, 0x00ff0055); /* UART_BB rts/cts/in/out */
	/* for LCDC */
	iowrite32(grf+GRF_GPIO1D_IOMUX, 0x00550055); /* LCDC0 DCLK/DEN/VSYNC/HSYNC */
	iowrite32(grf+GRF_GPIO1H_SR, 0x0f000f00); /* GPIO1C/D fast */
	iowrite32(grf+GRF_GPIO1D_E, 0x00ff00ff); /* GPIO1D drive strength */
	iowrite32(grf+GRF_SOC_CON6, 0x00080000); /* grf_con_lvds_lcdc_sel(VOP_BIG) */
	iowrite32(grf+GRF_SOC_CON7, 0x1fff1840); /* grf_lvds_con_enable_2/grf_lvds_con_enable_1/grf_lvds_con_ttl_en */
	/* for LVDS (for LCDC) */
	iowrite32(lvds+0x00*4, 0x7f);
	iowrite32(lvds+0x01*4, 0x40);
	iowrite32(lvds+0x02*4, 0x00);
	iowrite32(lvds+0x03*4, 0x46);
	iowrite32(lvds+0x04*4, 0x3f);
	iowrite32(lvds+0x05*4, 0x3f);
	iowrite32(lvds+0x0d*4, 0x0a);
	iowrite32(lvds+0x40*4, 0x7f);
	iowrite32(lvds+0x41*4, 0x40);
	iowrite32(lvds+0x42*4, 0x00);
	iowrite32(lvds+0x43*4, 0x46);
	iowrite32(lvds+0x44*4, 0x3f);
	iowrite32(lvds+0x45*4, 0x3f);
	iowrite32(lvds+0x4d*4, 0x0a);

	/* PMU GPIO */
	iowrite32(pmu+0x60/*PMU_GPIO_SR*/, 0x0007ffff);

	iowrite32(pmu+0x6c/*PMU_GPIO0_C_PULL*/, 0x3c);
	iowrite32(pmu+0x78/*PMU_GPIO0_C_DRV*/, 0x3f);
	iowrite32(pmu+0x8c/*PMU_GPIO0_C_IOMUX*/, 0x00000001); /* I2C0PMU_SCL */

	ioset32mask(pmu+0x68/*PMU_GPIO0_B_PULL*/, 0x00<<14, 0x03<<14);
	ioset32mask(pmu+0x74/*PMU_GPIO0_B_DRV*/, 0x03<<14, 0x03<<14);
	ioset32(pmu+0x88/*PMU_GPIO0_B_IOMUX*/, 0x01<<14); /* I2C0PMU_SDA */

	/* for LED */
	iowrite32(grf+GRF_GPIO8A_IOMUX, 0x003c0000); /* GPIO8A[2](POWER_LED)/[1](WORK_LED) */

	/* for SPI0/2 (SPI1 not used) */
	iowrite32(grf+GRF_GPIO5B_IOMUX, 0xff005500); /* SPI0:RXD/TXD/CSN0/CLK */
	iowrite32(grf+GRF_GPIO5C_IOMUX, 0x00030001); /* SPI0:CSN1 */
	iowrite32(grf+GRF_GPIO8A_IOMUX, 0xf0C05040); /* SPI2:CSN0/CLK/CSN1 */
	iowrite32(grf+GRF_GPIO8B_IOMUX, 0x000f0005); /* SPI2:TXD/RXD */

	/* for I2C */
	/* I2C2SENSOR_SCL/I2C2SENSOR_SDA */
	iowrite32(grf+GRF_GPIO8A_IOMUX, 0x0f000500);
	iowrite32(grf+GRF_GPIO8A_P, 0x0f000000);
	iowrite32(grf+GRF_GPIO8A_E, 0x0f000f00);
	/* I2C1AUDIO_SCL/I2C1AUDIO_SDA */
	iowrite32(grf+GRF_GPIO6B_IOMUX, 0x00140014);
	iowrite32(grf+GRF_GPIO6B_P, 0x003c0000);
	iowrite32(grf+GRF_GPIO6B_E, 0x003c003c);
	/* I2C3CAM_SCL/I2C3CAM_SDA */
	iowrite32(grf+GRF_GPIO2C_IOMUX, 0x00050005);
	iowrite32(grf+GRF_GPIO2C_P, 0x000f0000);
	iowrite32(grf+GRF_GPIO2C_E, 0x000f000f);
	/* I2C4TP_SCL/I2C4TP_SDA I2C5HDMI_SCL(7CH)/I2C5HDMI_SDA */
	iowrite32(grf+GRF_GPIO7CL_IOMUX, 0x03111110);
	iowrite32(grf+GRF_GPIO7CH_IOMUX, 0x00030001);
	iowrite32(grf+GRF_GPIO7C_P, 0x03fc0000);
	iowrite32(grf+GRF_GPIO7C_E, 0x03fc03fc);

	/* SDMMC0 */
	iowrite32(grf+GRF_GPIO6C_IOMUX, 0x17ff1555);
	iowrite32(grf+GRF_SOC_CON0, 0x10001000);

	/* register device */
	uart_register(&uart_info, arrayof(uart_info));
	gpio_register(&gpio_info, arrayof(gpio_info));
	spi_register(&spi_info, arrayof(spi_info));
	i2c_register(&i2c_info, arrayof(i2c_info));
	sdmmc_register(&sdmmc_info, arrayof(sdmmc_info));
}
