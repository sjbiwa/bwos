/*
 * Copyright (C) 2016 sjbiwa All rights reserved.
 */

/*
 * my_board.h
 *
 *  Created on: 2016/03/13
 *      Author: biwa
 */

#ifndef MY_BOARD_H_
#define MY_BOARD_H_

#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

#define	START_MEM_ADDR		(0x80000000u)
#define	END_MEM_ADDR		(0x8FFFFFFFu)

#define	UART_BASE			(0xFF1A0000u)

/* GIC */
#define	GIC_GICD_BASE		(0x00FEE00000)
#define	GIC_GICR_BASE		(0x00FEF00000)
#define	GIC_GICC_BASE		(0x00FFF00000)

#define	IRQ_NUM				(288)

#define	CACHE_LINE_SIZE		(64)

#define	CPU_PER_CLUSTER		(4)				/* 1クラスタあたり4コア */

#define	GENERIC_TIMER_INTR	(30)			/* genericTimerの割り込み番号 */

#define	CPU_CLOCK			(1000000000)	/* CPUクロック(未使用) */
#define	CNTFRQ_VALUE		(19200000)		/* GenericTimerのクロック */
#define	TIME_UNIT_VALUE		(1)				/* 1TIME-UNITのクロック数 */

/*** Tick管理パラメータ ***/
typedef	uint64_t			TimeSpec;		/* OS内部での時間単位 */
typedef	uint32_t			TimeOut;		/* APIで指定するタイムアウト単位 */

/* TIME_UNIT値に追従すること */
#define	SEC(n)				((TimeOut)(n) * (CNTFRQ_VALUE/TIME_UNIT_VALUE))
#define	MSEC(n)				((TimeOut)(n) * ((CNTFRQ_VALUE/1000)/TIME_UNIT_VALUE))
#define	USEC(n)				((TimeOut)(n) * ((CNTFRQ_VALUE/1000000)/TIME_UNIT_VALUE))
#define	NSEC(n)				((TimeOut)1)

#ifdef __cplusplus
}
#endif

#endif /* MY_BOARD_H_ */
