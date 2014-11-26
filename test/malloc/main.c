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

static void memchk(void* ptr, uint32_t val, uint32_t size)
{
	printf("CHK=%08X\n", ptr);
	uint8_t* cptr = ptr;
	for (; 0 < size; size--, cptr++) {
		if ( *cptr != val ) {
			printf("error\n");
			break;
		}
	}
}

void* mem_ptr[100];

int main()
{
	int ix;
	sys_malloc_init();
	sys_malloc_add_block(mem_block, 65536);
	dump_space();

	void* ptr1 = sys_malloc(54);
	memset(ptr1, 0x11, 54);
	void* ptr2 = sys_malloc(66);
	memset(ptr2, 0x22, 66);
	void* ptr3 = sys_malloc(72);
	memset(ptr3, 0x33, 72);
	void* ptr4 = sys_malloc(89);
	memset(ptr4, 0x44, 89);
	void* ptr5 = sys_malloc(111);
	memset(ptr5, 0x55, 111);

	memchk(ptr1, 0x11, 54);
	memchk(ptr2, 0x22, 66);
	memchk(ptr3, 0x33, 72);
	memchk(ptr4, 0x44, 89);
	memchk(ptr5, 0x55, 111);

	dump_space();
	dump_use(ptr1);
	dump_use(ptr2);
	dump_use(ptr3);
	dump_use(ptr4);
	dump_use(ptr5);

	sys_free(ptr2);
	dump_space();
	memchk(ptr1, 0x11, 54);
	memchk(ptr3, 0x33, 72);
	memchk(ptr4, 0x44, 89);
	memchk(ptr5, 0x55, 111);

	sys_free(ptr4);
	dump_space();
	memchk(ptr1, 0x11, 54);
	memchk(ptr3, 0x33, 72);
	memchk(ptr5, 0x55, 111);

	sys_free(ptr3);
	dump_space();
	memchk(ptr1, 0x11, 54);
	memchk(ptr5, 0x55, 111);

	sys_free(ptr5);
	dump_space();
	memchk(ptr1, 0x11, 54);

	sys_free(ptr1);
	dump_space();

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
