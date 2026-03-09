/*
 * Copyright (c) 2026 Jess Knue
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "proc.h"

//process list nodes were made to be a circular double-link list, for ease of
//access to both the next process and inserting a new process at the end.
typedef struct
{
    proc* process;
    struct scheduleNode* next;
    struct scheduleNode* prev;
} scheduleNode;

typedef struct
{
    struct scheduleNode* readyQueue;
    struct scheduleNode* sleepQueue;
    proc* currentProc;
} sched;

//initialize the scheduler, run on creation with struct
void schedulerInit();

//add new process to queue
void scheduleProcess(proc* process);

//switch process
void procSwitch();

//put sleeping processes on sleep queue
void procToSleep(struct scheduleNode* processNode);

//put process back on ready queue from sleep queue
void procToReady(struct scheduleNode* processNode);

//dead process handling function

#endif