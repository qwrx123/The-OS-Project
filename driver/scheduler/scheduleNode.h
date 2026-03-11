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

//initiallize new schedule nodes with the process they point to.
void scheduleNodeInit(scheduleNode* node, proc* process);

//add a schedule node to a schedule node list.
void addProc(scheduleNode* head, scheduleNode* newNode);

//remove and return the specified node in the list.
scheduleNode* dequeue(scheduleNode* targetProc);

#endif