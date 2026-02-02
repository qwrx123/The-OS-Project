/*
 * Copyright (c) 2026 Jess Knue
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "proc.h"
#include "proc_list.h"

//ready queue
//sleep queue

//add new process to queue
void procReady();
//switch process
void procSwitch();
//put sleeping processes on sleep queue
void procSleep();

#endif