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
	[CLOCK_SPI0]	= { 96000000 },
	[CLOCK_SPI1]	= {        0 },	/* not used */
	[CLOCK_SPI2]	= { 96000000 },
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

void init_task_board_depend(void)
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

	iowrite32(pmu+0x6c/*PMU_GPIO0_C_PULL*/, 0x3ff);
	iowrite32(pmu+0x78/*PMU_GPIO0_C_DRV*/, 0x3f);
	iowrite32(pmu+0x8c/*PMU_GPIO0_C_IOMUX*/, 0x00000000);

	/* for LED */
	iowrite32(grf+GRF_GPIO8A_IOMUX, 0x003c0000); /* GPIO8A[2](POWER_LED)/[1](WORK_LED) */

	/* for SPI0/2 (SPI1 not used) */
	iowrite32(grf+GRF_GPIO5B_IOMUX, 0xff005500); /* SPI0:RXD/TXD/CSN0/CLK */
	iowrite32(grf+GRF_GPIO5C_IOMUX, 0x00030001); /* SPI0:CSN1 */
	iowrite32(grf+GRF_GPIO8A_IOMUX, 0xf0C05040); /* SPI2:CSN0/CLK/CSN1 */
	iowrite32(grf+GRF_GPIO8B_IOMUX, 0x000f0005); /* SPI2:TXD/RXD */

	/* register device */
	uart_register(&uart_info, arrayof(uart_info));
	gpio_register(&gpio_info, arrayof(gpio_info));
	spi_register(&spi_info, arrayof(spi_info));
}
