/*
 * Copyright (c) 2026 Jess Knue
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "proc.h"
#include "../driver/scheduler/scheduleNode.h"

typedef struct sched
{
    struct scheduleNode* readyQueue;
    struct scheduleNode* sleepQueue;
    proc* currentProc;
} sched;

//initialize the scheduler, run on creation with struct
sched* schedulerInit(sched* scheduler);

//add new process to queue
sched* scheduleProcess(sched* scheduler, proc* process);

//switch process
void procSwitch(sched* scheduler);

proc* getNextProcess(sched* scheduler);

proc* getLastProcess(sched* scheduler);

proc* getNextSleepProcess(sched* scheduler);

proc* getLastSleepProcess(sched* scheduler);

#endif