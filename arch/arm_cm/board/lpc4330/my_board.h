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

#define	START_MEM_ADDR		(0x10000000u)
#define	END_MEM_ADDR		(0x1001FFFFu)

#define	IRQ_NUM				(256)

#define	CACHE_LINE_SIZE		(64)

#define	CPU_CLOCK			(72000000)/* CPUクロック */

/*** Tick管理パラメータ ***/
typedef	uint64_t	TimeSpec;	/* OS内部での時間単位 */
typedef	uint32_t	TimeOut;	/* APIで指定するタイムアウト単位 */

/* TIME_UNIT値に追従すること */
#define	SEC(n)		((n)*1000)
#define	MSEC(n)		(n)
#define	USEC(n)		(1)
#define	NSEC(n)		(1)

#endif /* MY_BOARD_H_ */
