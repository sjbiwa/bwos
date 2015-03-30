/*
 * boards.c
 *
 *  Created on: 2015/03/31
 *      Author: biwa
 */

#include "bwos.h"
#include "driver/clock.h"
#include "driver/uart.h"

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
		{ 0xff180000, 87, CLOCK_UART0},	/* UART_BT  */
		{ 0xff190000, 88, CLOCK_UART1},	/* UART_BB  */
		{ 0xff690000, 89, CLOCK_UART2},	/* UART_DBG */
		{ 0xff1b0000, 90, CLOCK_UART3},	/* UART_GPS */
		{ 0xff1c0000, 91, CLOCK_UART4},	/* UART_EXP */
};

void init_task_board_depend(void)
{
	/* pin configure */

	/* clock configure */

	clock_register(clock_params, arrayof(clock_params));
	uart_register(&uart_info, arrayof(uart_info));
}
