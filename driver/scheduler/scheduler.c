/*
 * Copyright (c) 2026 Jess Knue
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "scheduler.h"
#include "proc.h"
#include "uart.h"
#include "memallc.h"

extern void reg_switch(context outgoing, context incoming);

static scheduleNode *readyQueue;
static scheduleNode *sleepQueue;
static scheduleNode *currentProc;

void schedulerInit()
{
	uart_puts("scheduler: Initializing scheduler.\r\n");

	uart_puts("scheduler: Setting ready queue.\r\n");
	readyQueue = memallc(sizeof(scheduleNode));
	readyQueue->next = readyQueue;
	readyQueue->prev = readyQueue;

	uart_puts("scheduler: Setting sleep queue.\r\n");
	sleepQueue = memallc(sizeof(scheduleNode));
	sleepQueue->next = sleepQueue;
	sleepQueue->prev = sleepQueue;

	currentProc = 0;

	uart_puts("scheduler: Scheduler initiallized\r\n");
}

void procToSleep(scheduleNode *processNode)
{
	queueProc(sleepQueue, processNode);
	uart_puts("scheduler: Process put on sleep queue.\r\n");
}

void procToReady(scheduleNode *processNode)
{
	queueProc(readyQueue, processNode);
	uart_puts("scheduler: Process put on ready queue.\r\n");
}

void scheduleProcess(proc *process)
{
	uart_puts("scheduler: Creating schedule node for process.\r\n");
	scheduleNode *node = memallc(sizeof(scheduleNode));
	scheduleNodeInit(node, process);
	uart_puts("scheduler: Schedule node created.\r\n");

	uart_puts("scheduler: Checking process state...\r\n");
	if (process->proc_state == READY)
	{
		if (currentProc == 0)
		{
			process->proc_state = RUNNING;
			currentProc = node;
			uart_puts(
				"scheduler: No current process set. New process set to currently running.\r\n");
		}
		else
		{
			procToReady(node);
		}
	}
	else if (process->proc_state == SLEEPING)
	{
		procToSleep(node);
	}
}

void rescheduleProcess(scheduleNode *node)
{
	uart_puts("scheduler: Checking process state...\r\n");
	if (node->process->proc_state == READY)
	{
		procToReady(node);
	}
	else if (node->process->proc_state == SLEEPING)
	{
		procToSleep(node);
	}
	else if (node->process->proc_state == ZOMBIE)
	{
		uart_puts("scheduler: Process is a zombie. Kill it.\r\n");
		endProcess(node);
	}
}

void procSwitch()
{
	uart_puts("scheduler: Attempting process switch.\r\n");
	if (readyQueue->next == readyQueue)
	{
		uart_puts(
			"scheduler: No process waiting on ready queue, current process remains running.\r\n");
		return;
	}

	if (currentProc->process->proc_state == RUNNING)
	{
		currentProc->process->proc_state = READY;
	}

#ifndef TESTING //new issue: linker can't find definition when building for hardware
	//reg_switch(scheduler->currentProc->process->proc_context,
	//	   scheduler->readyQueue->next->process->proc_context);
#endif

	rescheduleProcess(currentProc);

	currentProc = dequeueProc(readyQueue->next);

	uart_puts("scheduler: Switched to next available process\r\n");
}

void awakenProcess()
{
	if (sleepQueue->next == sleepQueue)
	{
		uart_puts(
			"schedule: No processes waiting in sleep queue to awaken.\r\n");
		return;
	}

	uart_puts("scheduler: Awakening sleeping process.\r\n");
	sleepQueue->next->process->proc_state = READY;
	rescheduleProcess(dequeueProc(sleepQueue->next));
}

void endProcess(scheduleNode *processNode)
{
	free_memallc(processNode->process);
	free_memallc(processNode);
	uart_puts("scheduler: Process terminated.");
}

void killScheduler()
{
	while (readyQueue->next != readyQueue)
	{
		free_memallc(readyQueue->next->process);
		free_memallc(dequeueProc(readyQueue->next));
	}

	while (sleepQueue->next != sleepQueue)
	{
		free_memallc(sleepQueue->next->process);
		free_memallc(dequeueProc(sleepQueue->next));
	}

	free_memallc(readyQueue);
	free_memallc(sleepQueue);
	free_memallc(currentProc->process);
	free_memallc(currentProc);

	uart_puts("scheduler: Scheduler killed, all related memory freed.\r\n");
}

void scheduleNodeInit(scheduleNode *node, proc *process)
{
	node->process = process;
	node->next = node;
	node->prev = node;
}

//for schedulenodes
void queueProc(scheduleNode *head, scheduleNode *newNode)
{
	newNode->next = head;
	newNode->prev =
		head->prev; //head->prev is equal to head when head is "empty"
	head->prev->next =
		newNode; //head->prev->next is equal to head->next when head is "empty"
	head->prev = newNode;
}

//for schedulenodes
scheduleNode *dequeueProc(scheduleNode *targetProc)
{
	targetProc->next->prev = targetProc->prev;
	targetProc->prev->next = targetProc->next;
	targetProc->next = targetProc;
	targetProc->prev = targetProc;

	return targetProc;
}

void testingSchedulerInit(scheduleNode *ready, scheduleNode *sleep,
			  scheduleNode *curProc)
{
	ready->next = ready;
	ready->prev = ready;
	readyQueue = ready;
	sleep->next = sleep;
	sleep->prev = sleep;
	sleepQueue = sleep;
	currentProc = curProc;
}

scheduleNode *getNextProcess()
{
	return readyQueue->next;
}

scheduleNode *getLastProcess()
{
	return readyQueue->prev;
}

scheduleNode *getNextSleepProcess()
{
	return sleepQueue->next;
}

scheduleNode *getLastSleepProcess()
{
	return sleepQueue->prev;
}

scheduleNode *getRunningProcess()
{
	return currentProc;
}