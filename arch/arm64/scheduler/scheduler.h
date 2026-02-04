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

//add new process to queue
void procReady(proc* process)
{
    node* newProcess;
    newProcess->process = process;
    addProc(newProcess);
}

//switch process
void procSwitch();

//put sleeping processes on sleep queue
void procSleep();

//put process back on ready queue from sleep queue
void procWake();

#endif