/*
 * memory.c
 *
 *  Created on: 2014/11/27
 *      Author: biwa
 */
#include <stdint.h>

void memset(void* ptr, uint32_t val, uint32_t len)
{
	uint8_t* cptr = (uint8_t*)ptr;
	for (; 0 < len; len--, cptr++) {
		*cptr = val;
	}
}

void memcpy(void* dst, void* src, uint32_t len)
{
	uint8_t* c_dst = dst;
	uint8_t* c_src = src;
	for (; 0 < len; len--) {
		*c_dst++ = *c_src++;
	}
}
