/*
 * task.h
 *
 *  Created on: 2012/12/01
 *      Author: biwa
 */

#ifndef TASK_H_
#define TASK_H_

#include "common.h"

#define	TASK_FRAME_SIZE		(4*18)	/* タスク情報退避エリアのサイズ(スタック上) */
#define	TASK_FRAME_PC		(16)	/* タスク情報退避エリアのPC位置 */
#define	TASK_FRAME_PSR		(17)	/* タスク情報退避エリアのCPSR位置 */
#define	TASK_FRAME_STUB		(9)		/* タスク情報退避エリアの割り込みハンドラスタブ位置 */

#ifndef __ASM__
typedef	enum { TASK_READY, TASK_WAIT, TASK_DONE } TaskState;
typedef	struct tagTaskStruct {
	/* Fixed Position */
	Link		link;				/* ReadQueue/EventQueue LinkList */
	void*		start_entry;		/* Start Entry */
	void*		save_sp;			/* Current SP */
	/******************/
	uint32_t	stack_size;			/* Stack Size */
	uint8_t		name[32];			/* Task Name */
	uint32_t	priority;			/* Task Priority */
	uint32_t	taskid;				/* TaskID */
	TaskState	task_state;			/* Task State */
	Link		tlink;				/* TimeOut LinkList */
	uint32_t	timeout;			/* TimeOut Time */
} TaskStruct ;

extern	TaskStruct*			_ctask;
extern	TaskStruct*			_ntask;

#else
#define	START_ENTRY			(0)
#define	SAVE_SP				(12)
#endif

#endif /* TASK_H_ */
