/*
 * Copyright (c) 2026 Jess Knue
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */
 
 #include "scheduler.h"

void schedulerInit(sched* scheduler)
{
    scheduler->readyQueue->next = scheduler->readyQueue;
    scheduler->readyQueue->prev = scheduler->readyQueue;
    scheduler->sleepQueue->next = scheduler->sleepQueue;
    scheduler->sleepQueue->prev = scheduler->sleepQueue;
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
    //call on timer interrupt
    //if readyQueue->next != readyQueue
    //save context of current proc
    //load context of readyQueue->next
    //put current proc on ready (or sleep) queue
    //remove readyQueue->next and hold it in currentProc
}


//not yet implemented
void procToSleep(node* processNode)
{
    addProc(sleepQueue, processNode);
}


//not yet implemented
void procToReady(node* processNode)
{
    addProc(readyQueue, processNode);
}