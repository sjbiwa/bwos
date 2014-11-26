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

void dump_space(void);
void dump_use(void* ptr);

uint8_t mem_block[65536] __attribute__((aligned(16)));

void* mem_ptr[100];

int main()
{
	int ix;
	sys_malloc_init();
	sys_malloc_add_block(mem_block, 65536);

	for (;;) {
		for (ix=0; ix<100; ix++) {
			mem_ptr[ix] = sys_malloc(128+ix*4);
			memset(mem_ptr[ix], ix, 128+ix*4);
		}

		for (ix=0; ix<100; ix+=2) {
			sys_free(mem_ptr[ix]);
			dump_space();
		}
		for (ix=1; ix<100; ix+=2) {
			sys_free(mem_ptr[ix]);
			dump_space();
		}
		dump_space();
		sleep(3);
	}

	return 0;
}
