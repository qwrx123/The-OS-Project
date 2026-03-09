/*
 * Copyright (c) 2026 Jess Knue
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */
 
#include "scheduler.h"
#include "proc.h"

extern void reg_switch();

static sched* scheduler;

void schedulerInit()
{
    scheduler->readyQueue->next = scheduler->readyQueue;
    scheduler->readyQueue->prev = scheduler->readyQueue;
    scheduler->sleepQueue->next = scheduler->sleepQueue;
    scheduler->sleepQueue->prev = scheduler->sleepQueue;
}


void scheduleProcess(proc* process)
{
    scheduleNode* newProcess;
    newProcess->process = process;

    if(process->proc_state == READY)
    {
        procToReady(newProcess);
    }
    else if(process->proc_state == SLEEPING)
    {
        procToSleep(newProcess);
    }
}


//not yet implemented
void procSwitch()
{
    //is called on through timer interrupt or 
    if (scheduler->readyQueue->next != scheduler->readyQueue)
    {
        reg_switch(scheduler->currentProc->proc_context, scheduler->readyQueue->next->proc->proc_context);
        if (scheduler->currentProc->proc_state == READY)
        {
            procToReady(scheduler->currentProc);
        }
        else
        {
            procToSleep(scheduler->currentProc);
        }

        scheduler->currentProc = scheduler->readyQueue->next;
    }
    //remove readyQueue->next and hold it in currentProc
}


//not yet implemented
void procToSleep(scheduleNode* processNode)
{
    addProc(scheduler->sleepQueue, processNode);
}


//not yet implemented
void procToReady(scheduleNode* processNode)
{
    addProc(scheduler->readyQueue, processNode);
}


int isEmpty(scheduleNode* head)
{
    return head->next == head;
}


void addProc(scheduleNode* head, scheduleNode* newNode)
{
    if(isEmpty(head))
    {
        head->next = newNode;
        head->prev = newNode;
        newNode->next = head;
        newNode->prev = head;
    }
    else
    {
        newNode->next = head;
        newNode->prev = head->prev;
        head->prev = newNode;
    }
}


void removeProc(scheduleNode* targetProc)
{
    targetProc->next->prev = targetProc->prev;
    targetProc->prev->next = targetProc->next;

    //if process state READY: addProc ReadyQueue
    //if process state SLEEPING: addProc SleepQueue
    //if process state ZOMBIE: targetProc->process.kill
}