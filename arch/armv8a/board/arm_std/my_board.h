/*
 * Copyright (C) 2015 sjbiwa All rights reserved.
 */

/*
 * my_board.h
 *
 *  Created on: 2015/12/13
 *      Author: biwa
 */

#ifndef MY_BOARD_H_
#define MY_BOARD_H_

#include "stdint.h"

#define	START_MEM_ADDR		(0x80000000u)
#define	END_MEM_ADDR		(0x8FFFFFFFu)

#define	SYSTEM_BASE			(0x1C010000u)

#define	GIC_GICD_BASE		(0x002F000000)
#define	GIC_GICR_BASE		(0x002F100000)
#define	GIC_GICC_BASE		(0x002C002000)

#define	IRQ_NUM				(256)

#define	CACHE_LINE_SIZE		(64)

#define	CPU_CLOCK			(1000000000)	/* CPUクロック */
#define	CNTFRQ_VALUE		(100000000)		/* GenericTimerのクロック */
#define	TIME_UNIT_VALUE		(0x8000)		/* 1TIME-UNITのクロック数 */

/*** Tick管理パラメータ ***/
typedef	uint64_t			TimeSpec;		/* OS内部での時間単位 */
typedef	uint32_t			TimeOut;		/* APIで指定するタイムアウト単位 */

/* TIME_UNIT値に追従すること */
#define	SEC(n)				((TimeOut)(n) * (CNTFRQ_VALUE/TIME_UNIT_VALUE))
#define	MSEC(n)				((TimeOut)(n) * ((CNTFRQ_VALUE/1000)/TIME_UNIT_VALUE))
#define	USEC(n)				((TimeOut)1)
#define	NSEC(n)				((TimeOut)1)

#endif /* MY_BOARD_H_ */
