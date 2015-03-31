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

#define	CLOCK_UART0			0
#define	CLOCK_UART1			1
#define	CLOCK_UART2			2
#define	CLOCK_UART3			3
#define	CLOCK_UART4			4

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

	/* clock configure */

	clock_register(clock_params, arrayof(clock_params));
	uart_register(&uart_info, arrayof(uart_info));
}
