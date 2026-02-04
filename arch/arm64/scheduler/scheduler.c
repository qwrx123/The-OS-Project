/*
 * Copyright (c) 2026 Jess Knue
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */
 
 #include "scheduler.h"

void schedulerInit(sched* scheduler)
{
    scheduler->readyQueue->next = readyQueue;
    scheduler->readyQueue->prev = readyQueue;
    scheduler->sleepQueue->next = sleepQueue;
    scheduler->sleepQueue->prev = sleepQueue;
}


void scheduleProcess(proc* process)
{
    node* newProcess;
    newProcess->process = process;

    if(process->state == READY)
    {
        readyProc(newProcess);
    }
    else if(process->state == SLEEPING)
    {
        sleepProc(newProcess);
    }
}


void procSwitch()
{

}


void procToSleep()
{

}


void procToReady()
{

}