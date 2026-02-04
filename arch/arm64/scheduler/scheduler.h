/*
 * Copyright (c) 2026 Jess Knue
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "proc.h"
#include "proc_list.h"

typedef struct
{
    node* readyQueue;
    node* sleepQueue;
    proc* currentProc;
} sched;

//initialize the scheduler, run on creation with struct
void schedulerInit();

//add new process to queue
void scheduleProc(proc* process);

//switch process
void procSwitch();

//put sleeping processes on sleep queue
void procToSleep();

//put process back on ready queue from sleep queue
void procToReady();

//dead process handling function

#endif