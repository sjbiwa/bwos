/*
 * sem.c
 *
 *  Created on: 2015/01/04
 *      Author: biwa
 */

#include "common.h"
#include "task.h"
#include "sem.h"
#include "link.h"
#include "api.h"

OSAPI int sem_create(SemStruct* sem, uint32_t max)
{
	link_clear(&(sem->link));
	sem->max = sem->remain = max;
	sem->insufficient = 0;
	return 0;
}

OSAPI int sem_request2(SemStruct* sem, uint32_t num)
{
	int ret = RT_ERR;
	uint32_t		cpsr;
	if ( num <= sem->max ) {
		irq_save(cpsr);
		if ( num <= sem->remain ) {
			/* 全数取得できる場合 */
			sem->remain -= num;
			ret = RT_OK;
		}
		else {
			/* 獲得数不足の場合 */

		}
	}
	return ret;
}

OSAPI int sem_request(SemStruct* sem)
{
	return sem_request2(sem, 1);
}

OSAPI int sem_release2(SemStruct* sem, uint32_t num)
{

}

OSAPI int sem_release(SemStruct* sem)
{
	return sem_release2(sem, 1);
}
