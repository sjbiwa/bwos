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

int main()
{
	sys_malloc_init();
	sys_malloc_add_block(mem_block, 65536);
	dump_space();

	void* ptr1 = sys_malloc(128);
	dump_space();
	dump_use(ptr1);
	void* ptr2 = sys_malloc(128);
	void* ptr3 = sys_malloc(128);
	void* ptr4 = sys_malloc(128);
	void* ptr5 = sys_malloc(128);
	sys_free(ptr2);
	dump_space();
	sys_free(ptr4);
	dump_space();
	sys_free(ptr3);
	dump_space();
	sys_free(ptr5);
	dump_space();
	sys_free(ptr1);
	dump_space();

	return 0;
}
