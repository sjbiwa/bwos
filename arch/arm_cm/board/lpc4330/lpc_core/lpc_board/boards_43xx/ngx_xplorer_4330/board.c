/*
 * Copyright(C) NXP Semiconductors, 2012
 * All rights reserved.
 *
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */

#include "board.h"
#include "string.h"

#include "retarget.h"

/** @ingroup BOARD_NGX_XPLORER_18304330
 * @{
 */

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/* System configuration variables used by chip driver */
const uint32_t ExtRateIn = 0;
const uint32_t OscRateIn = 12000000;

static void Board_LED_Init()
{
	/* P2.12 : LED D2 as output */
	Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, 1, 12);

	/* P2.11 : LED D3 as output */
	Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, 1, 11);

	/* Set initial states to off (true to disable) */
	Chip_GPIO_SetPinState(LPC_GPIO_PORT, 1, 12, (bool) true);
	Chip_GPIO_SetPinState(LPC_GPIO_PORT, 1, 11, (bool) true);
}

void Board_LED_Set(uint8_t LEDNumber, bool On)
{
	if (LEDNumber == 0) {
		Chip_GPIO_SetPinState(LPC_GPIO_PORT, 1, 12, (bool) !On);
	}
	else if (LEDNumber == 1) {
		Chip_GPIO_SetPinState(LPC_GPIO_PORT, 1, 11, (bool) !On);
	}
}

bool Board_LED_Test(uint8_t LEDNumber)
{
	if (LEDNumber == 0) {
		return (bool) !Chip_GPIO_GetPinState(LPC_GPIO_PORT, 1, 12);
	}
	else if (LEDNumber == 1) {
		return (bool) !Chip_GPIO_GetPinState(LPC_GPIO_PORT, 1, 11);
	}

	return false;
}

void Board_LED_Toggle(uint8_t LEDNumber)
{
	Board_LED_Set(LEDNumber, !Board_LED_Test(LEDNumber));
}

void Board_Buttons_Init(void)	// FIXME not functional ATM
{
	Chip_SCU_PinMuxSet(0x2, 7, (SCU_MODE_PULLUP | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS | SCU_MODE_FUNC0));		// P2_7 as GPIO0[7]
	Chip_GPIO_SetPinDIRInput(LPC_GPIO_PORT, BUTTONS_BUTTON1_GPIO_PORT_NUM, (1 << BUTTONS_BUTTON1_GPIO_BIT_NUM));	// input
}

uint32_t Buttons_GetStatus(void)
{
	uint8_t ret = NO_BUTTON_PRESSED;
	if (Chip_GPIO_GetPinState(LPC_GPIO_PORT, BUTTONS_BUTTON1_GPIO_PORT_NUM, BUTTONS_BUTTON1_GPIO_BIT_NUM) == 0) {
		ret |= BUTTONS_BUTTON1;
	}
	return ret;
}


/* Set up and initialize all required blocks and functions related to the
   board hardware */
void Board_Init(void)
{
	/* Initializes GPIO */
	Chip_GPIO_Init(LPC_GPIO_PORT);

	/* Initialize LEDs */
	Board_LED_Init();
}


/**
 * @}
 */
