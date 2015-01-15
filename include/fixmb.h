/*
 * fixmb.h
 *
 *  Created on: 2015/01/16
 *      Author: biwa
 */

#ifndef FIXMB_H_
#define FIXMB_H_

typedef struct tagFixmbStruct {
	Link		link;
	void*		mblock;
	uint32_t	mb_size;
	uint32_t	mb_length;
} FixmbStruct;


#endif /* FIXMB_H_ */
