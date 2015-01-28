/*
 * api_caller.c
 *
 *  Created on: 2015/01/29
 *      Author: biwa
 */
#include "api.h"
#include "api_caller_callee.h"

OSAPI int flag_twait(FlagStruct* flag, uint32_t pattern, uint32_t wait_mode, uint32_t* ret_pattern, TimeOut tmout)
{
	extern int flag_twait_stub(APIFlagTwaitParam*);
	APIFlagTwaitParam param;
	param.flag = flag;
	param.pattern = pattern;
	param.wait_mode = wait_mode;
	param.tmout = tmout;
	return flag_twait_stub(&param);
}
