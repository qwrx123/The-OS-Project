/*
 * Copyright (c) 2026 Jess Knue
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef PROC_LIST_H
#define PROC_LIST_H

#include "proc.h"

struct node
{
    proc* process;
    node* next = NULL;
    node* prev = NULL;
};

node* head = NULL;

void addProc(proc* newProcess)
{
    node* newNode;
    newNode->process = newProcess;

    if(isEmpty())
    {
        head = newNode;
    }
    else
    {
        newNode->next = head;
        newNode->prev = head->prev;
        head->prev = newNode;
        
        if(head->next == NULL)
        {
            head->next = newNode;
        }
    }
}


void removeProc(node* targetProc)
{
    targetProc->next->prev = targetProc->prev;
    targetProc->prev->next = targetProc->next;
}


bool isEmpty()
{
    return head == NULL;
}

#endif