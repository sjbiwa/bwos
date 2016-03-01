/*
 * Copyright (C) 2015 sjbiwa All rights reserved.
 */

/*
 * memmgr.c
 *
 *  Created on: 2014/12/07
 *      Author: biwa
 */
#ifndef _MEMMGR_H_
#define	_MEMMGR_H_

#include "kernel.h"
#include "arm.h"
#include "mpcore.h"
#include "cp15reg.h"

#ifdef __cplusplus
extern "C" {
#endif

enum {
	ATTRL_SECT = 0,
	ATTRL_PAGE,
};
enum {
	ATTR_TEXT = 0,
	ATTR_DATA,
	ATTR_DEV,
};

extern void arch_ptbl_init(void);
extern void arch_ptbl_enable(void);
extern void mmgr_add_entry(void* addr, MemSize_t size, uint32_t attr);

#ifdef __cplusplus
}
#endif

#endif
