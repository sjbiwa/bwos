/*
 * main.c
 *
 *  Created on: 2014/11/24
 *      Author: biwa
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void sys_malloc_init(void);
void sys_malloc_add_block(void* addr, uint32_t size);
void* sys_malloc(uint32_t size);
void sys_free(void* ptr);

uint8_t mem_block[65536];

int main()
{
	sys_malloc_init();
	sys_malloc_add_block(mem_block, 65536);

	uint32_t size = 0;
	for (;;) {
		void* ptr = sys_malloc(128);
		if ( !ptr ) {
			break;
		}
		size += 128;
		memset(ptr, 0x55, 128);
		printf("ADDR=%08X TOTAL=%d\n", ptr, size);
	}
	return 0;
}
