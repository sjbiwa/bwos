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

typedef	struct tagFlagStruct {
	Link		link;
	uint32_t	value;
} FlagStruct;


#endif /* FLAG_H_ */
