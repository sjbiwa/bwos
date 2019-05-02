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
#include "nrfx_clock.h"

static void nrfx_clock_event_handler(nrfx_clock_evt_type_t event)
{
}

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
	nrfx_clock_init(nrfx_clock_event_handler);
	nrfx_clock_enable();
	nrfx_clock_hfclk_start();
	nrfx_clock_lfclk_start();
}

void board_register_normal_memory(void)
{
}
