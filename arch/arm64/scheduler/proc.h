/*
 * Copyright (c) 2026 Jess Knue
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef PROC_H
#define PROC_H

struct proc
{
    state proc_state;
    context proc_context;
};


typedef enum
{
    READY,
    RUNNING,
    SLEEPING,
    ZOMBIE
} state;


struct context{
    uint64_t sp; //Stack pointer
    uint64_t lr; //Link register
    uint64_t pc; //Program counter
    uint64_t apsr; //Application program status register
    //General registers
    uint64_t r0;
    uint64_t r1;
    uint64_t r2;
    uint64_t r3;
    uint64_t r4;
    uint64_t r5;
    uint64_t r6;
    uint64_t r7;
    uint64_t r8;
    uint64_t r9;
    uint64_t r10;
    uint64_t r11;
    uint64_t r12;
};

#endif