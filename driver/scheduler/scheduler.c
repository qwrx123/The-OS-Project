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

#define MAX_SCHEDULED_PROCESSES 24
static proc currentProc_s;
static proc readyQueue_s[MAX_SCHEDULED_PROCESSES];
static int readyQueue_head;
static int readyQueue_tail;
static proc sleepQueue_s[MAX_SCHEDULED_PROCESSES];
static int sleepQueue_head;
static int sleepQueue_tail;

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
	uart_puts("scheduler: Scheduler initiallized\r\n");
}

void procToSleep(sched *scheduler, scheduleNode *processNode)
{
	queueProc(scheduler->sleepQueue, processNode);
	uart_puts("scheduler: Process put on sleep queue.\r\n");
}

void procToReady(sched *scheduler, scheduleNode *processNode)
{
	queueProc(scheduler->readyQueue, processNode);
	uart_puts("scheduler: Process put on ready queue.\r\n");
}

void scheduleProcess(sched *scheduler, proc *process)
{
	scheduleNode *node = memallc(sizeof(scheduleNode));
	scheduleNodeInit(node, process);
	uart_puts("scheduler: Process given schedule node.\r\n");

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
		uart_puts("scheduler: Process running.\r\n");
	}
}

void procSwitch(sched *scheduler)
{
	if (scheduler->readyQueue->next == scheduler->readyQueue)
	{
		return;
	}

	scheduler->currentProc->process->proc_state = READY;

#ifndef TESTING //new issue: linker can't find definition when building for hardware
	//reg_switch(scheduler->currentProc->process->proc_context,
	//	   scheduler->readyQueue->next->process->proc_context);
#endif

	procToReady(scheduler, scheduler->currentProc);

	scheduler->currentProc = dequeueProc(scheduler->readyQueue->next);

	uart_puts("scheduler: Switched to next available process\r\n");
}

void killScheduler(sched *scheduler)
{
	while (scheduler->readyQueue->next != scheduler->readyQueue)
	{
		free_memallc(scheduler->readyQueue->next->process);
		free_memallc(dequeueProc(scheduler->readyQueue->next));
	}

	while (scheduler->sleepQueue->next != scheduler->sleepQueue)
	{
		free_memallc(scheduler->sleepQueue->next->process);
		free_memallc(dequeueProc(scheduler->sleepQueue->next));
	}

	free_memallc(scheduler->readyQueue);
	free_memallc(scheduler->sleepQueue);
	free_memallc(scheduler->currentProc->process);
	free_memallc(scheduler->currentProc);
	free_memallc(scheduler);

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

void schedulerInit_static()
{
	uart_puts("scheduler: Setting up queues.\r\n");
	readyQueue_head = 0;
	readyQueue_tail = 0;
	sleepQueue_head = 0;
	sleepQueue_tail = 0;
	uart_puts("scheduler: Setting initial process.\r\n");
	context c = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	currentProc_s = (proc){ RUNNING, c, -1 };
	uart_puts("scheduler: Scheduler initiallized.\r\n");
}

int rqIsFull()
{
	return readyQueue_head ==
	       (readyQueue_tail + 1) % MAX_SCHEDULED_PROCESSES;
}

int rqIsEmpty()
{
	return readyQueue_head == readyQueue_tail;
}

int sqIsFull()
{
	return sleepQueue_head ==
	       (sleepQueue_tail + 1) % MAX_SCHEDULED_PROCESSES;
}

int sqIsEmpty()
{
	return sleepQueue_head == sleepQueue_tail;
}

void procToReady_s(proc process)
{
	if (!rqIsFull())
	{
		readyQueue_s[readyQueue_tail++] = process;
		uart_puts("scheduler: Process added to ready queue.\r\n");
	}
	else
	{
		uart_puts(
			"scheduler: Ready Queue is full, process not scheduled.\r\n");
	}
}

void procToSleep_s(proc process)
{
	if (!sqIsFull())
	{
		sleepQueue_s[sleepQueue_tail++] = process;
		uart_puts("scheduler: Process added to sleep queue.\r\n");
	}
	else
	{
		uart_puts(
			"scheduler: Sleep queue is full, process not scheduled.\r\n");
	}
}

void scheduleProcess_static(proc process)
{
	uart_puts(
		"scheduler: Scheduling process. Checking process state...\r\n");
	if (process.proc_state == READY)
	{
		procToReady_s(process);
	}
	else if (process.proc_state == SLEEPING)
	{
		procToSleep_s(process);
	}
	else if (process.proc_state == ZOMBIE)
	{
		uart_puts(
			"scheduler: Process is zombie, removed from scheduling.\r\n");
	}
}

void procSwitch_static()
{
	uart_puts("scheduler: Preparing to swap processes\r\n");
	if (rqIsEmpty())
	{
		uart_puts(
			"scheduler: No new process to schedule. Current process remains active.\r\n");
		return;
	}

	if (currentProc_s.proc_state == RUNNING)
	{
		uart_puts(
			"scheduler: Setting \"running\" process state to \"ready\".\r\n");
		currentProc_s.proc_state = READY;
	}

#ifndef TESTING
	//reg_switch(currentProc_s.proc_context, readyQueue_s[readyQueue_head].proc_context);
#endif

	scheduleProcess_static(currentProc_s);
	currentProc_s = readyQueue_s[readyQueue_head++];
	currentProc_s.proc_state = RUNNING;
	uart_puts("scheduler: Processes swapped, new process running.\r\n");
}

void procAwaken_static()
{
	if (sqIsEmpty())
	{
		uart_puts(
			"scheduler: No process on sleep queue to awaken.\r\n");
	}

	uart_puts("scheduler: Awakening sleeping process.\r\n");
	sleepQueue_s[sleepQueue_head].proc_state = READY;
	scheduleProcess_static(sleepQueue_s[sleepQueue_head++]);
}

void killProcess_static()
{
	uart_puts("scheduler: Killing active process.\r\n");
	currentProc_s.proc_state = ZOMBIE;
	procSwitch_static();
}

proc getNextProcess_s()
{
	return readyQueue_s[readyQueue_head];
}

proc getLastProcess_s()
{
	return readyQueue_s[(readyQueue_tail - 1) % MAX_SCHEDULED_PROCESSES];
}

proc getNextSleepProcess_s()
{
	return sleepQueue_s[sleepQueue_head];
}

proc getLastSleepProcess_s()
{
	return sleepQueue_s[(sleepQueue_tail - 1) % MAX_SCHEDULED_PROCESSES];
}

proc getRunningProcess_s()
{
	return currentProc_s;
}