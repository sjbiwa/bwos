/*
 * flag.h
 *
 *  Created on: 2014/11/18
 *      Author: biwa
 */

#ifndef FLAG_H_
#define FLAG_H_

#include <stdint.h>
#include "link.h"

/* フラグ関連API wait_mode */
#define	FLAG_OR			(0x00000001u<<0)
#define	FLAG_AND		(0x00000001u<<1)
#define	FLAG_CLR		(0x00000001u<<2)
#define	FLAG_BITCLR		(0x00000001u<<3)


typedef	struct tagFlagStruct {
	Link		link;
	uint32_t	value;
} FlagStruct;


#endif /* FLAG_H_ */
