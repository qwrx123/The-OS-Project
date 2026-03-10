/*
 * Copyright (c) 2026 Jess Knue
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "proc.h"

typedef struct sched
{
    struct scheduleNode* readyQueue;
    struct scheduleNode* sleepQueue;
    proc* currentProc;
} sched;

//initialize the scheduler, run on creation with struct
void schedulerInit(sched* scheduler);

//add new process to queue
void scheduleProcess(sched* scheduler, proc* process);

//switch process
void procSwitch(sched* scheduler);

#endif