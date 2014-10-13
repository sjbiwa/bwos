/*
 * common.h
 *
 *  Created on: 2012/12/01
 *      Author: biwa
 */

#ifndef COMMON_H_
#define COMMON_H_
#include "config.h"
#include "types.h"

#define	NULL			(0)


#ifndef __ASM__
#include <stdint.h>
#include <stddef.h>

typedef	struct tagLink {
	struct tagLink*		next;
	struct tagLink*		prev;
} Link;

#define	iowrite32(a,d)		(*((volatile uint*)(a))=(d))
#define	ioread32(a)			(*((volatile uint*)(a)))

#define	arrayof(a)			(sizeof(a)/sizeof((a)[0]))

extern uint32_t				tick_count;

#define	containerof(ptr, type, member) ({ const typeof(((type *)0)->member) *__mptr = (ptr);(type *)((char*)__mptr-offsetof(type,member));})

static inline void Link_clear(Link* link) { link->next = link->prev = NULL; }

#else
#endif

#endif /* COMMON_H_ */
