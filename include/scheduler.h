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
    scheduleNode* readyQueue;
    scheduleNode* sleepQueue;
    scheduleNode* currentProc;
} sched;

//initialize the scheduler, run on creation with struct
void schedulerInit(sched* scheduler);

//add new process to queue
void scheduleProcess(sched* scheduler, proc* process);

//switch process
void procSwitch(sched* scheduler);

//method for deallocating all processes and scheduleNodes within the scheduler,
//then the scheduler itself.
void killScheduler(sched* scheduler);

//for testing. returns the process in the next ready node.
proc* getNextProcess(sched* scheduler);

//for testing. returns the process of the last added ready node.
proc* getLastProcess(sched* scheduler);

//for testing. returns the process in the next sleeping node.
proc* getNextSleepProcess(sched* scheduler);

//for testing. returns the process of the last added sleeping node.
proc* getLastSleepProcess(sched* scheduler);

proc* getRunningProcess(sched* scheduler);

#endif