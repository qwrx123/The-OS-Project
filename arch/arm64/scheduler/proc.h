/*
 * Copyright (c) 2026 Jess Knue
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef PROC_H
#define PROC_H

typedef enum
{
    READY,
    RUNNING,
    SLEEPING,
    ZOMBIE
} state;

struct proc
{
    state proc_state;
    context proc_context;
};

#endif