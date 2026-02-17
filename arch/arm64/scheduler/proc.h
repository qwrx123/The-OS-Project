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
    uint64_t SP; //Stack pointer
    uint64_t PSTATE; //Processor state
    uint64_t x30; //Link register
    uint64_t x29; //Frame pointer
    uint64_t x18; //Global pointer

    uint64_t x0;

    uint64_t x1;
    uint64_t x2;
    uint64_t x3;
    uint64_t x4;
    uint64_t x5;
    uint64_t x6;
    uint64_t x7;

    uint64_t x8;
    uint64_t x9;
    uint64_t x10;
    uint64_t x11;
    uint64_t x12;
    uint64_t x13;
    uint64_t x14;
    uint64_t x15;
    uint64_t x16;
    uint64_t x17;
    
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
};

#endif