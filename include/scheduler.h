/*
 * Copyright (c) 2026 Jess Knue
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "proc.h"

typedef struct scheduleNode
{
	proc *process;
	struct scheduleNode *next;
	struct scheduleNode *prev;
} scheduleNode;

typedef struct sched
{
	scheduleNode *readyQueue;
	scheduleNode *sleepQueue;
	scheduleNode *currentProc;
} sched;

/**
 * @brief Initializes the scheduler
 */
void schedulerInit();

/**
 * @brief Add new process to the scheduler's queues
 * @param process Pointer to the new process struct to add
 */
void scheduleProcess(proc *process);

/**
 * @brief Included in header for testing while scheduleProcess segfaults. Puts schedule node (process) on the ready queue
 * @param processNode Pointer to the schedule node containing the process to queue
 */
void procToReady(scheduleNode *processNode);

/**
 * @brief Included in header for testing while scheduleProcess segfaults. Puts schedule node (process) on the sleep queue
 * @param processNode Pointer to the schedule node containing the process to queue
 */
void procToSleep(scheduleNode *processNode);

/**
 * @brief Switches active process
 */
void procSwitch();

/**
 * @brief Frees all allocated memory for the scheduler
 */
void killScheduler();

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
void queueProc(scheduleNode *head, scheduleNode *newNode);

/**
 * @brief Dequeues the specified schedule node from it's queue
 * @param targetProc Pointer to the specified schedule node to be dequeued
 * @return Pointer to the now dequeued schedule node
 */
scheduleNode *dequeueProc(scheduleNode *targetProc);

/**
 * @brief For testing purposes. Get the next process in the ready queue
 * @return Pointer to the process contained in the next schedule node
 */
proc *getNextProcess();

/**
 * @brief For testing purposes. Get the process last added to the ready queue
 * @return Pointer to the process contained in the last schedule node
 */
proc *getLastProcess();

/**
 * @brief For testing purposes. Get the next process in the sleep queue
 * @return Pointer to the process contained in the next schedule node
 */
proc *getNextSleepProcess();

/**
 * @brief For testing purposes. Get the process last added to the sleep queue
 * @return Pointer to the process contained in the last schedule node
 */
proc *getLastSleepProcess();

/**
 * @brief For testing purposes. Get the currently running process of the scheduler
 * @return Pointer to the currently running process
 */
proc *getRunningProcess();

//functions below will mirror functions above, but for a static scheduler,
//no dynamic memory allocation needed

void schedulerInit_static();
void scheduleProcess_static(proc process);
void procSwitch_static();
void procAwaken_static();
void killProcess_static();

proc getNextProcess_s();
proc getLastProcess_s();
proc getNextSleepProcess_s();
proc getLastSleepProcess_s();
proc getRunningProcess_s();

#endif