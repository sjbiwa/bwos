/*
 * Copyright (C) 2015 syujibiwa All rights reserved.
 */

/*
 * boards.c
 *
 *  Created on: 2015/07/01
 *      Author: biwa
 */

#include "bwos.h"

void init_task_board_depend(void)
{
	/************************/
	/* clock configure		*/
	/************************/

	/************************/
	/* pin configure		*/
	/************************/
}

void board_system_preinit(void)
{
	Board_SystemInit();
	Board_Init();
}
