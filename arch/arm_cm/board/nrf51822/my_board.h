/*
 * Copyright (C) 2015 sjbiwa All rights reserved.
 */

/*
 * my_board.h
 *
 *  Created on: 2015/07/01
 *      Author: biwa
 */

#ifndef MY_BOARD_H_
#define MY_BOARD_H_

#include "stdint.h"
#include "cmsis.h"

#define	START_MEM_ADDR		(0x20000000u)
#define	END_MEM_ADDR		(0x20003F00u)

#define	IRQ_NUM				(32+16)

#define	CACHE_LINE_SIZE		(64)

#define	CPU_CLOCK			(MAX_CLOCK_FREQ)/* CPUクロック */

#define	TICK_TIME			(1)		/* tick周期 10ms */

/*** Tick管理パラメータ ***/
typedef	uint64_t			TimeSpec;	/* OS内部での時間単位 */
typedef	uint32_t			TimeOut;	/* APIで指定するタイムアウト単位 */

/* TIME_UNIT値に追従すること */
#define	SEC(n)		((n)*1000000)
#define	MSEC(n)		((n)*1000)
#define	USEC(n)		((n))
#define	NSEC(n)		(1)

#endif /* MY_BOARD_H_ */
