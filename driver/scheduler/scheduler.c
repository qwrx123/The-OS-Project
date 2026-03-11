/*
 * Copyright (c) 2026 Jess Knue
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */
 
#include "scheduler.h"
#include "proc.h"

extern void reg_switch(context outgoing, context incoming);

sched* schedulerInit(sched* scheduler)
{
    static scheduleNode ready = {0, 0, 0};
    static scheduleNode sleep = {0, 0, 0};
    ready.next = &ready;
    ready.prev = &ready;
    sleep.next = &sleep;
    sleep.prev = &sleep;
    scheduler->readyQueue = &ready;
    scheduler->sleepQueue = &sleep;
    return scheduler;
}

void procToSleep(sched* scheduler, scheduleNode* processNode)
{
    addProc(scheduler->sleepQueue, processNode);
}

void procToReady(sched* scheduler, scheduleNode* processNode)
{
    addProc(scheduler->readyQueue, processNode);
}

sched* scheduleProcess(sched* scheduler, proc* process)
{
    scheduleNode node = {0, 0, 0};
    scheduleNodeInit(&node, process);

    if(process->proc_state == READY)
    {
        procToReady(scheduler, &node);
    }
    else if(process->proc_state == SLEEPING)
    {
        procToSleep(scheduler, &node);
    }

    return scheduler;
}

void procSwitch(sched* scheduler)
{
    if (scheduler->readyQueue->next != scheduler->readyQueue)
    {
        scheduler->currentProc->proc_state = READY;
        //reg_switch(scheduler->currentProc->proc_context, scheduler->readyQueue->next->process->proc_context);
        scheduleProcess(scheduler, scheduler->currentProc);

        scheduler->currentProc = dequeue(scheduler->readyQueue->next);
    }
}

proc* getNextProcess(sched* scheduler)
{
    return scheduler->readyQueue->next->process;
}

proc* getLastProcess(sched* scheduler)
{
    return scheduler->readyQueue->prev->process;
}

proc* getNextSleepProcess(sched* scheduler)
{
    return scheduler->sleepQueue->next->process;
}

proc* getLastSleepProcess(sched* scheduler)
{
    return scheduler->sleepQueue->prev->process;
}