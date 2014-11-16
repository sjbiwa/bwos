/*
 * flag.c
 *
 *  Created on: 2014/11/15
 *      Author: biwa
 */

typedef	struct tagFlagStruct {
	uint32_t		value;
} FlagStruct;

OSAPI int flag_create(FlagStruct* flag)
{
	return 0;
}

OSAPI void flag_set(FlagStruct* flag)
{

}

OSAPI void flag_wait(FlagStruct* flag)
{

}
