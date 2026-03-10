/*
 * Copyright (c) 2026 Jess Knue
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */
 
#include "scheduler.h"
#include "scheduleNode.h"
#include "proc.h"

extern void reg_switch();

void schedulerInit(sched* scheduler)
{
    scheduler->readyQueue->next = scheduler->readyQueue;
    scheduler->readyQueue->prev = scheduler->readyQueue;
    scheduler->sleepQueue->next = scheduler->sleepQueue;
    scheduler->sleepQueue->prev = scheduler->sleepQueue;
}


//not yet implemented
void procToSleep(sched* scheduler, scheduleNode* processNode)
{
    addProc(scheduler->sleepQueue, processNode);
}


//not yet implemented
void procToReady(sched* scheduler, scheduleNode* processNode)
{
    addProc(scheduler->readyQueue, processNode);
}


void scheduleProcess(sched* scheduler, proc* process)
{
    scheduleNode node = {process, 0, 0};
    scheduleNode* newProcess = {&node};
    scheduleNodeInit(newProcess, process);

    if(process->proc_state == READY)
    {
        procToReady(scheduler, newProcess);
    }
    else if(process->proc_state == SLEEPING)
    {
        procToSleep(scheduler, newProcess);
    }
}


//not yet implemented
void procSwitch(sched* scheduler)
{
    //is called on through timer interrupt or 
    if (scheduler->readyQueue->next != scheduler->readyQueue)
    {
        reg_switch(scheduler->currentProc->proc_context, scheduler->readyQueue->next->process->proc_context);
        scheduleProcess(scheduler, scheduler->currentProc);

        scheduler->currentProc = scheduler->readyQueue->next->process;
    }
    //remove readyQueue->next and hold it in currentProc
}