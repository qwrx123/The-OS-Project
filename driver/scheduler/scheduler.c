/*
 * Copyright (c) 2026 Jess Knue
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "scheduler.h"
#include "proc.h"
#include "memallc.h"

extern void reg_switch(context outgoing, context incoming);

//does not work, segfaults at memory allocation (even if malloc used instead)
void schedulerInit(sched *scheduler)
{
	scheduleNode *ready = memallc(sizeof(scheduleNode));
	scheduleNode *sleep = memallc(sizeof(scheduleNode));
	ready->next = ready;
	ready->prev = ready;
	sleep->next = sleep;
	sleep->prev = sleep;
	scheduler->readyQueue = ready;
	scheduler->sleepQueue = sleep;
}

void procToSleep(sched *scheduler, scheduleNode *processNode)
{
	addProc(scheduler->sleepQueue, processNode);
}

void procToReady(sched *scheduler, scheduleNode *processNode)
{
	addProc(scheduler->readyQueue, processNode);
}

//does not work, segfaults at memory allocation (even if malloc used instead)
void scheduleProcess(sched *scheduler, proc *process)
{
	scheduleNode *node = memallc(sizeof(scheduleNode));
	scheduleNodeInit(node, process);

	if (process->proc_state == READY)
	{
		procToReady(scheduler, node);
	}
	else if (process->proc_state == SLEEPING)
	{
		procToSleep(scheduler, node);
	}
	else if (process->proc_state == RUNNING)
	{
		//for testing purposes. should probably make an actual "if empty make process run" thing
		scheduler->currentProc = node;
	}
}

void procSwitch(sched *scheduler)
{
	if (scheduler->readyQueue->next == scheduler->readyQueue)
	{
		return;
	}

	scheduler->currentProc->process->proc_state = READY;
	reg_switch(scheduler->currentProc->process->proc_context,
		   scheduler->readyQueue->next->process->proc_context);
	procToReady(scheduler, scheduler->currentProc);

	scheduler->currentProc = dequeue(scheduler->readyQueue->next);
}

void killScheduler(sched *scheduler)
{
	while (scheduler->readyQueue->next != scheduler->readyQueue)
	{
		free_memallc(scheduler->readyQueue->next->process);
		free_memallc(dequeue(scheduler->readyQueue->next));
	}

	while (scheduler->sleepQueue->next != scheduler->sleepQueue)
	{
		free_memallc(scheduler->sleepQueue->next->process);
		free_memallc(dequeue(scheduler->sleepQueue->next));
	}

	free_memallc(scheduler->readyQueue);
	free_memallc(scheduler->sleepQueue);
	free_memallc(scheduler->currentProc->process);
	free_memallc(scheduler->currentProc);
	free_memallc(scheduler);
}

proc *getNextProcess(sched *scheduler)
{
	return scheduler->readyQueue->next->process;
}

proc *getLastProcess(sched *scheduler)
{
	return scheduler->readyQueue->prev->process;
}

proc *getNextSleepProcess(sched *scheduler)
{
	return scheduler->sleepQueue->next->process;
}

proc *getLastSleepProcess(sched *scheduler)
{
	return scheduler->sleepQueue->prev->process;
}

proc *getRunningProcess(sched *scheduler)
{
	return scheduler->currentProc->process;
}