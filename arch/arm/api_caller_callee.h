/*
 * api_caller_callee.h
 *
 *  Created on: 2015/01/29
 *      Author: biwa
 */
#include <stdint.h>
#include "common.h"
#include "api.h"

#ifndef _API_CALLER_CALLEE_H_
#define _API_CALLER_CALLEE_H_

/* ARMは４つの引数までしかレジスタ渡しできないので５つ以上 */
/* 引数があるAPIについては CALLER-CALLEEスタブを使う */

/*OSAPI int flag_twait(FlagStruct* flag, uint32_t pattern, uint32_t wait_mode, uint32_t* ret_pattern, TimeOut tmout);*/
typedef	struct {
	FlagStruct* flag;
	uint32_t pattern;
	uint32_t wait_mode;
	uint32_t* ret_pattern;
	TimeOut tmout;
} APIFlagTwaitParam;

#endif /* _API_CALLER_CALLEE_H_ */
