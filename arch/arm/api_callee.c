/*
 * Copyright (C) 2015 sjbiwa All rights reserved.
 */

/*
 * api_callee.c
 *
 *  Created on: 2015/01/29
 *      Author: biwa
 */

#include "kernel.h"
#include "api_caller_callee.h"

int __flag_twait_stub(APIFlagTwaitParam* param)
{
	return __flag_twait(param->id, param->pattern, param->wait_mode, param->ret_pattern, param->tmout);
}
