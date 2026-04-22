/*
 * Copyright (c) 2026 Jess Knue
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef PROC_H
#define PROC_H

#include "kernel/types.h"

typedef enum state
{
	READY,
	RUNNING,
	SLEEPING,
	ZOMBIE
} state;

typedef struct context
{
	uint64_t SP; //Stack pointer

	uint64_t x19;
	uint64_t x20;
	uint64_t x21;
	uint64_t x22;
	uint64_t x23;
	uint64_t x24;
	uint64_t x25;
	uint64_t x26;
	uint64_t x27;
	uint64_t x28;

	uint64_t x29; //Frame pointer
	uint64_t x30; //Link register
} context;

typedef struct proc
{
	state proc_state;
	context proc_context;
} proc;

#endif