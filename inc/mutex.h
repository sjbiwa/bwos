/*
 * mutex.h
 *
 *  Created on: 2014/11/19
 *      Author: biwa
 */

#ifndef MUTEX_H_
#define MUTEX_H_

#include "task.h"
#include "link.h"

typedef	struct tagMutexStruct {
	Link		link;
	TaskStruct*	task;	/* 獲得中のタスク */
	uint32_t	count;	/* 獲得回数 */
} MutexStruct;


#endif /* MUTEX_H_ */
