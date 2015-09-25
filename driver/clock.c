/*
 * Copyright (C) 2015 sjbiwa All rights reserved.
 */

/*
 * clock.c
 *
 *  Created on: 2015/03/30
 *      Author: biwa
 */

#include "bwos.h"
#include "driver/clock.h"

static ClockRegisterParam* clock_list;
static uint32_t clock_list_num;

uint32_t clock_get(int clk_id)
{
	return clock_list[clk_id].freq;
}

void clock_register(ClockRegisterParam* list, uint32_t list_num)
{
	clock_list = list;
	clock_list_num = list_num;
}
