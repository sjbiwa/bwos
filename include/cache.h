/*
 * Copyright (C) 2016 sjbiwa All rights reserved.
 */

/*
 * cache.h
 *
 *  Created on: 2016/02/04
 *      Author: biwa
 */

#ifndef _CACHE_H_
#define _CACHE_H_

#include <stdint.h>

/* 以下はアーキテクチャ毎に用意すること */
extern void cache_sync(void);
extern void cache_invalid(void* addr, uint32_t size);
extern void cache_invalid_unalign(void* addr, uint32_t size);
extern void cache_clean(void* addr, uint32_t size);
extern void cache_clean_unalign(void* addr, uint32_t size);
extern void cache_clean_invalid(void* addr, uint32_t size);
extern void cache_clean_invalid_unalign(void* addr, uint32_t size);
/*****************************************/

static inline void cache_invalid_sync(void* addr, uint32_t size)
{
	cache_invalid(addr, size);
	cache_sync();
}

static inline void cache_invalid_unalign_sync(void* addr, uint32_t size)
{
	cache_invalid_unalign(addr, size);
	cache_sync();
}

static inline void cache_clean_sync(void* addr, uint32_t size)
{
	cache_clean(addr, size);
	cache_sync();
}

static inline void cache_clean_unalign_sync(void* addr, uint32_t size)
{
	cache_clean_unalign(addr, size);
	cache_sync();
}

static inline void cache_clean_invalid_sync(void* addr, uint32_t size)
{
	cache_clean_invalid(addr, size);
	cache_sync();
}

static inline void cache_clean_invalid_unalign_sync(void* addr, uint32_t size)
{
	cache_clean_invalid_unalign(addr, size);
	cache_sync();
}

#endif /* _CACHE_H_ */
