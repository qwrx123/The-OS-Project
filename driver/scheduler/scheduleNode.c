/*
 * Copyright (c) 2026 Jess Knue
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "scheduleNode.h"

void scheduleNodeInit(scheduleNode *node, proc *process)
{
	node->process = process;
	node->next = node;
	node->prev = node;
}

void addProc(scheduleNode *head, scheduleNode *newNode)
{
	newNode->next = head;
	newNode->prev =
		head->prev; //head->prev is equal to head when head is "empty"
	head->prev->next =
		newNode; //head->prev->next is equal to head->next when head is "empty"
	head->prev = newNode;
}

scheduleNode *dequeue(scheduleNode *targetProc)
{
	targetProc->next->prev = targetProc->prev;
	targetProc->prev->next = targetProc->next;

	return targetProc;
}