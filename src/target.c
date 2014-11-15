/*
 * target.c
 *
 *  Created on: 2014/11/15
 *      Author: biwa
 */
#include "comon.h"
#include "task.h"

static void delay(void)
{
	volatile int		ix, iy;
	for (ix=0x900000; 0<ix; ix--) {
		iy = ix / 234;
	}
}

void task1(void)
{
	int			ix;
	long long	llval1 = 0;
	long long	llval2 = 0;
	long long	llval3 = 0;
	long long	llval4 = 0;
	for (ix=0;;ix++) {
		llval1 += ix;
		llval2 += ix;
		llval3 += ix;
		llval4 += ix;
		irq_disable();
		tprintf("task1 : %d %08X\n", ix, ({uint sp;asm volatile ("mov %0, sp":"=r"(sp)::"memory");sp;}));
		tprintf("task1 : %08X %08X %08X %08X\n", &llval1, &llval2, &llval3, &llval4);
		irq_enable();
		task_tsleep(20);
	}
}

void task2(void)
{
	int			ix;
	int			iy;
	long long	llval1 = 0;
	long long	llval2 = 0;
	long long	llval3 = 0;
	long long	llval4 = 0;
	for (ix=0, iy=0;;ix++, iy+=3) {
		llval1 += ix;
		llval2 += ix;
		llval3 += ix;
		llval4 += ix;
		irq_disable();
		tprintf("task2 : %d %08X\n", iy, ({uint sp;asm volatile ("mov %0, sp":"=r"(sp)::"memory");sp;}));
		tprintf("task2 : %08X %08X %08X %08X\n", &llval1, &llval2, &llval3, &llval4);
		irq_enable();
		task_tsleep(25);
	}
}

void task3(void)
{
	int			ix;
	int			iy;
	long long	llval1 = 0;
	long long	llval2 = 0;
	long long	llval3 = 0;
	long long	llval4 = 0;
	for (ix=0, iy=0;;ix++, iy+=3) {
		llval1 += ix;
		llval2 += ix;
		llval3 += ix;
		llval4 += ix;
		irq_disable();
		tprintf("task3 : %d %08X\n", iy, ({uint sp;asm volatile ("mov %0, sp":"=r"(sp)::"memory");sp;}));
		tprintf("task3 : %08X %08X %08X %08X\n", &llval1, &llval2, &llval3, &llval4);
		irq_enable();
		task_tsleep(30);
	}
}

void task4(void)
{
	int			ix;
	int			iy;
	long long	llval1 = 0;
	long long	llval2 = 0;
	long long	llval3 = 0;
	long long	llval4 = 0;
	for (ix=0, iy=0;;ix++, iy+=3) {
		llval1 += ix;
		llval2 += ix;
		llval3 += ix;
		llval4 += ix;
		irq_disable();
		tprintf("task4 : %d %08X\n", iy, ({uint sp;asm volatile ("mov %0, sp":"=r"(sp)::"memory");sp;}));
		tprintf("task4 : %08X %08X %08X %08X\n", &llval1, &llval2, &llval3, &llval4);
		irq_enable();
		task_tsleep(35);
	}
}

void task5(void)
{
	int			ix;
	int			iy;
	long long	llval1 = 0;
	long long	llval2 = 0;
	long long	llval3 = 0;
	long long	llval4 = 0;
	for (ix=0, iy=0;;ix++, iy+=3) {
		llval1 += ix;
		llval2 += ix;
		llval3 += ix;
		llval4 += ix;
		irq_disable();
		tprintf("task5 : %d %08X\n", iy, ({uint sp;asm volatile ("mov %0, sp":"=r"(sp)::"memory");sp;}));
		tprintf("task5 : %08X %08X %08X %08X\n", &llval1, &llval2, &llval3, &llval4);
		irq_enable();
		task_tsleep(40);
	}
}
