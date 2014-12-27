/*
 * my_board.h
 *
 *  Created on: 2014/12/07
 *      Author: biwa
 */

#ifndef MY_BOARD_H_
#define MY_BOARD_H_

#define	START_MEM_ADDR			(0x40000000u)
#define	END_MEM_ADDR			(0x7FFFFFFFu)

#define	CACHE_LINE_SIZE			(64)

#define	HAVE_CP15_TIMER

#define	CNTFRQ_VALUE			(24000000)
#define	CPU_CLOCK				(1000000000)
#define	TICK_CYCLE				(1)			/* ms単位 */

#endif /* MY_BOARD_H_ */
