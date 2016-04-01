/*
 * Copyright (C) 2015 sjbiwa All rights reserved.
 */

/*
 * clock.h
 *
 *  Created on: 2015/03/07
 *      Author: biwa
 */

#ifndef CLOCK_H_
#define CLOCK_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef	struct {
	uint32_t	freq;		/* クロックのベース周波数 */
} ClockRegisterParam;

extern uint32_t clock_get(int clk_id);
extern void clock_register(ClockRegisterParam* list, uint32_t list_num);

#ifdef __cplusplus
}
#endif

#endif /* CLOCK_H_ */
