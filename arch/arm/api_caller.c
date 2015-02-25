/*
 * api_caller.c
 *
 *  Created on: 2015/01/29
 *      Author: biwa
 */
#include "api.h"
#include "api_caller_callee.h"

OSAPI int flag_twait(int id, uint32_t pattern, uint32_t wait_mode, uint32_t* ret_pattern, TimeOut tmout)
{
	extern int flag_twait_stub(APIFlagTwaitParam*);
	APIFlagTwaitParam param;
	param.id = id;
	param.pattern = pattern;
	param.wait_mode = wait_mode;
	param.ret_pattern = ret_pattern;
	param.tmout = tmout;
	return flag_twait_stub(&param);
}
