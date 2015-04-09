/*
 * boards.c
 *
 *  Created on: 2015/03/31
 *      Author: biwa
 */

#include "bwos.h"
#include "driver/clock.h"
#include "driver/uart.h"
#include "irqdefs.h"
#include "ioregs.h"
#include "cruregs.h"
#include "grfregs.h"

#define	CLOCK_UART0			0
#define	CLOCK_UART1			1
#define	CLOCK_UART2			2
#define	CLOCK_UART3			3
#define	CLOCK_UART4			4

static const uint32_t	cru = CRU_REG_BASE;
static const uint32_t	grf = GRF_REG_BASE;

static ClockRegisterParam clock_params[] = {
	[CLOCK_UART0] = { 24000000 },
	[CLOCK_UART1] = { 24000000 },
	[CLOCK_UART2] = { 24000000 },
	[CLOCK_UART3] = { 24000000 },
	[CLOCK_UART4] = { 24000000 },
};

static UartDeviceInfo uart_info[] = {
		{ UART_BT_BASE,  IRQ_UART_BT,  CLOCK_UART0},	/* UART_BT  */
		{ UART_BB_BASE,  IRQ_UART_BB,  CLOCK_UART1},	/* UART_BB  */
		{ UART_DBG_BASE, IRQ_UART_DBG, CLOCK_UART2},	/* UART_DBG */
		{ UART_GPS_BASE, IRQ_UART_GPS, CLOCK_UART3},	/* UART_GPS */
		{ UART_EXP_BASE, IRQ_UART_EXP, CLOCK_UART4},	/* UART_EXP */
};

void init_task_board_depend(void)
{
	/* pin configure */
	iowrite32(grf+GRF_GPIO5B_IOMUX, 0x00ff0055); /* UART_BB rts/cts/in/out */
	iowrite32(grf+GRF_GPIO1D_IOMUX, 0x00550055); /* LCDC0 DCLK/DEN/VSYNC/HSYNC */
	iowrite32(grf+GRF_SOC_CON6, 0x00080008); /* grf_con_lvds_lcdc_sel */
	iowrite32(grf+GRF_SOC_CON7, 0xa0000000); /* grf_lvds_pwrdwn/grf_lvds_lcdc_trace_sel */
	iowrite32(grf+GRF_IO_VSEL, 0x00010000); /* lcdc_v18sel */

	/* clock configure */
	iowrite32(cru+CRU_CLKSEL14_CON, 0xffff0200); /* UART_BB / 24MHz */
	iowrite32(cru+CRU_CLKSEL16_CON, 0xffff0200); /* UART_GPS / 24MHz */
	iowrite32(cru+CRU_CLKSEL3_CON,  0xffff0200); /* UART_EXP / 24MHz */

	clock_register(clock_params, arrayof(clock_params));
	uart_register(&uart_info, arrayof(uart_info));
}
