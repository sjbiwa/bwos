/*
 * Copyright (C) 2015 sjbiwa All rights reserved.
 */

/*
 * api_caller_callee.h
 *
 *  Created on: 2015/01/29
 *      Author: biwa
 */
#include "common.h"

#ifndef _API_CALLER_CALLEE_H_
#define _API_CALLER_CALLEE_H_

#ifdef __cplusplus
extern "C" {
#endif

/* ARMは４つの引数までしかレジスタ渡しできないので５つ以上 */
/* 引数があるAPIについては CALLER-CALLEEスタブを使う */

/*OSAPI int flag_twait(int id, uint32_t pattern, uint32_t wait_mode, uint32_t* ret_pattern, TimeOut tmout);*/
typedef	struct {
	int id;
	uint32_t pattern;
	uint32_t wait_mode;
	uint32_t* ret_pattern;
	TimeOut tmout;
} APIFlagTwaitParam;
extern int __flag_twait_stub(APIFlagTwaitParam* param);

#ifdef __cplusplus
}
#endif

#endif /* _API_CALLER_CALLEE_H_ */
