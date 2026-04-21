/*
 * Copyright (c) [2026] Jorden Wilsey Bacso
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef TIMER_H
#define TIMER_H

#include "kernel/types.h"

/*
uint64_t count = CNTPCT_EL0;
uint64_t freq = CNTFRQ_EL0;
uint64_t interupt_timer = CNTPS_CVAL_EL1;
*/

uint64_t freq;
uint64_t time;

// typedef struct

/**
 * @brief Initializes the timer.
 */
void init_timer();

/**
 * @brief Sets bit[2] to 1, bit[1] to 0, and bit[0] of CNTPS_CTL_EL1 to 1 to enable the timer
 */
void enable_timer();

/**
 * @brief Sets bit[2] to 1, bit[1] to 0, and bit[0] of CNTPS_CTL_EL1 to 0 to disable the timer
 */
void disable_timer();

/**
 * @brief Handles the timer interrupt.
 */
void timer_interrupt();

/**
 * @brief Gets the current timer value.
 * @return The current timer value.
 */
uint64_t get_time();

/**
 * @brief Gets the timer frequency.
 * @return The timer frequency.
 */
uint64_t get_timer_freq();

#endif