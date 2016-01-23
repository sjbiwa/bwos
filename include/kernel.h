/*
 * Copyright (C) 2015 sjbiwa All rights reserved.
 */

/*
 * kernel.h
 *
 *  Created on: 2015/03/07
 *      Author: biwa
 */

#ifndef KERNEL_H_
#define KERNEL_H_

#include "common.h"
#include "kernel/kernel_api.h"
#include "kernel/fixmb.h"
#include "kernel/flag.h"
#include "kernel/malloc.h"
#include "kernel/msgq.h"
#include "kernel/mutex.h"
#include "kernel/sem.h"
#include "kernel/task.h"
#include "kernel/kobject.h"
#include "kernel/arch_kernel_api.h"
#include "interface.h"
#include "lock.h"
#include "link.h"

/* 初期タスクの生成パラメータ(arch側で用意する) */
extern TaskCreateInfo	_init_task_create_param;

#endif /* KERNEL_H_ */
