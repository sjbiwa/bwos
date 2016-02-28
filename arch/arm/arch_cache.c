/*
 * Copyright (C) 2016 sjbiwa All rights reserved.
 */
/*
 * arch_cache.c
 *
 *  Created on: 2016/02/04
 *      Author: biwa
 */

#include "bwos.h"
#include <stdint.h>
#include "arch_common.h"

void cache_sync(void)
{
	__dsb();
}

void cache_invalid(void* addr, uint32_t size)
{
	PtrInt_t saddr = (PtrInt_t)addr;
	PtrInt_t eaddr = (PtrInt_t)addr + size;
	for (; saddr < eaddr; saddr += CACHE_LINE_SIZE ) {
		DCIMVAC_set(saddr);
	}
}

void cache_invalid_unalign(void* addr, uint32_t size)
{
	PtrInt_t saddr = (PtrInt_t)addr;
	PtrInt_t eaddr = (PtrInt_t)addr + size;

	/* キャッシュアラインかチェック */
	if ( saddr & (CACHE_LINE_SIZE - 1) ) {
		/* 非アライン */
		DCCIMVAC_set(saddr);
		/* saddrを次のキャッシュアラインに調整 */
		saddr &= ~(PtrInt_t)(CACHE_LINE_SIZE - 1);
		saddr += CACHE_LINE_SIZE;
	}

	if ( saddr < eaddr ) {
		/* キャッシュアラインかチェック */
		if ( eaddr & (CACHE_LINE_SIZE - 1) ) {
			/* 非アライン */
			DCCIMVAC_set(eaddr);
			/* eaddrを前のキャッシュアラインに調整 */
			eaddr &= ~(PtrInt_t)(CACHE_LINE_SIZE - 1);
		}

		for (; saddr < eaddr; saddr += CACHE_LINE_SIZE ) {
			DCIMVAC_set(saddr);
		}
	}
}

void cache_clean(void* addr, uint32_t size)
{
	PtrInt_t saddr = (PtrInt_t)addr;
	PtrInt_t eaddr = (PtrInt_t)addr + size;
	for (; saddr < eaddr; saddr += CACHE_LINE_SIZE ) {
		DCCMVAC_set(saddr);
	}
}

void cache_clean_unalign(void* addr, uint32_t size)
{
	PtrInt_t left = (PtrInt_t)addr & (CACHE_LINE_SIZE - 1);
	cache_clean((void*)((PtrInt_t)addr - left), size + left);
}

void cache_clean_invalid(void* addr, uint32_t size)
{
	PtrInt_t saddr = (PtrInt_t)addr;
	PtrInt_t eaddr = (PtrInt_t)addr + size;
	for (; saddr < eaddr; saddr += CACHE_LINE_SIZE ) {
		DCCIMVAC_set(saddr);
	}
}

void cache_clean_invalid_unalign(void* addr, uint32_t size)
{
	PtrInt_t left = (PtrInt_t)addr & (CACHE_LINE_SIZE - 1);
	cache_clean_invalid((void*)((PtrInt_t)addr - left), size + left);
}

