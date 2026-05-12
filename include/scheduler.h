/*
 * Copyright (c) 2026 Jess Knue
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "proc.h"
#include "../driver/scheduler/scheduleNode.h"

typedef struct sched
{
	scheduleNode *readyQueue;
	scheduleNode *sleepQueue;
	scheduleNode *currentProc;
} sched;

/**
 * @brief Initializes the scheduler
 * @param scheduler Pointer to the scheduler's sched struct
 */
void schedulerInit(sched *scheduler);

/**
 * @brief Add new process to the scheduler's queues
 * @param scheduler Pointer to sched struct
 * @param process Pointer to the new process struct to add
 */
void scheduleProcess(sched *scheduler, proc *process);

/**
 * @brief Included in header for testing while scheduleProcess segfaults. Puts schedule node (process) on the ready queue
 * @param scheduler Pointer to the sched struct
 * @param processNode Pointer to the schedule node containing the process to queue
 */
void procToReady(sched *scheduler, scheduleNode *processNode);

/**
 * @brief Included in header for testing while scheduleProcess segfaults. Puts schedule node (process) on the sleep queue
 * @param scheduler Pointer to the sched struct
 * @param processNode Pointer to the schedule node containing the process to queue
 */
void procToSleep(sched *scheduler, scheduleNode *processNode);

/**
 * @brief Switches active process
 * @param scheduler Pointer to the sched struct
 */
void procSwitch(sched *scheduler);

/**
 * @brief Frees all allocated memory for the scheduler
 * @param scheduler Pointer to the sched struct
 */
void killScheduler(sched *scheduler);

/**
 * @brief For testing purposes. Get the next process in the ready queue
 * @param scheduler Pointer to the sched struct
 * @return Pointer to the process contained in the next schedule node
 */
proc *getNextProcess(sched *scheduler);

/**
 * @brief For testing purposes. Get the process last added to the ready queue
 * @param scheduler Pointer to the sched struct
 * @return Pointer to the process contained in the last schedule node
 */
proc *getLastProcess(sched *scheduler);

/**
 * @brief For testing purposes. Get the next process in the sleep queue
 * @param scheduler Pointer to the sched struct
 * @return Pointer to the process contained in the next schedule node
 */
proc *getNextSleepProcess(sched *scheduler);

/**
 * @brief For testing purposes. Get the process last added to the sleep queue
 * @param scheduler Pointer to the sched struct
 * @return Pointer to the process contained in the last schedule node
 */
proc *getLastSleepProcess(sched *scheduler);

/**
 * @brief For testing purposes. Get the currently running process of the scheduler
 * @param scheduler Pointer to the sched struct
 * @return Pointer to the currently running process
 */
proc *getRunningProcess(sched *scheduler);

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