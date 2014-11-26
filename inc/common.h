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
#include "arch.h"

#define	NULL			(0)


#ifndef __ASM__
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* ユーティリティマクロ */
#define	arrayof(a)			(sizeof(a)/sizeof((a)[0]))
#define	containerof(ptr, type, member) ({ const typeof(((type *)0)->member) *__mptr = (ptr);(type *)((char*)__mptr-offsetof(type,member));})
#define	iowrite32(a,d)		(*((volatile uint32_t*)(a))=(d))
#define	ioread32(a)			(*((volatile uint32_t*)(a)))


/* OS固有定義マクロ */
#define	OSAPI

#else
#endif

#endif /* COMMON_H_ */
