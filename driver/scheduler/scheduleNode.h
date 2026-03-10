/*
 * Copyright (c) 2026 Jess Knue
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */
 
#ifndef SCHEDULE_NODE_H
#define SCHEDULE_NODE_H

#include "proc.h"

//process list nodes were made to be a circular double-link list, for ease of
//access to both the next process and inserting a new process at the end.
typedef struct scheduleNode
{
    proc* process;
    struct scheduleNode* next;
    struct scheduleNode* prev;
} scheduleNode;


void scheduleNodeInit(scheduleNode* node, proc* process)
{
    node->process = process;
    node->next = node;
    node->prev = node;
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

#endif