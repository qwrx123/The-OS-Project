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
	proc *process;
	struct scheduleNode *next;
	struct scheduleNode *prev;
} scheduleNode;

/**
 * @brief Initiallize the schedule node
 * @param node Pointer to the scheduleNode struct
 * @param process Pointer to the process to be attached to the schedule node
 */
void scheduleNodeInit(scheduleNode *node, proc *process);

/**
 * @brief Add schedule node (containing process) to a linked list of schedule nodes
 * @param head Pointer to the head schedule node
 * @param newNode Pointer to the new schedule node to be added
 */
void addProc(scheduleNode *head, scheduleNode *newNode);

/**
 * @brief Dequeues the specified schedule node from it's queue
 * @param targetProc Pointer to the specified schedule node to be dequeued
 * @return Pointer to the now dequeued schedule node
 */
scheduleNode *dequeue(scheduleNode *targetProc);

#endif