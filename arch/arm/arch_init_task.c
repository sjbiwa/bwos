/*
 * Copyright (C) 2015 sjbiwa All rights reserved.
 */

/*
 * arch.c
 *
 *  Created on: 2014/12/07
 *      Author: biwa
 */

#include "bwos.h"

extern char __init_array_start;
extern char __init_array_end;

void init_global_ctor(void)
{
	for ( void (**p)() = &__init_array_start; p < &__init_array_end; p++ ) {
		(**p)();
	}
}

void arch_init_task_depend(void)
{
	init_global_ctor();
	board_init_task_depend();
}
