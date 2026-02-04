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
        procToReady(newProcess);
    }
    else if(process->state == SLEEPING)
    {
        procToSleep(newProcess);
    }
}


//not yet implemented
void procSwitch()
{

}


//not yet implemented
void procToSleep()
{

}


//not yet implemented
void procToReady()
{

}