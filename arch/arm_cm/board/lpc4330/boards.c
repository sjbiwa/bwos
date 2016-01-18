/*
 * Copyright (C) 2015 sjbiwa All rights reserved.
 */

/*
 * boards.c
 *
 *  Created on: 2015/07/01
 *      Author: biwa
 */

#include "bwos.h"

void board_init_task_depend(void)
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

void board_register_normal_memory(void)
{
}
