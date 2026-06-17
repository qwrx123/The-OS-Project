/*
 * Copyright (c) 2026 Nathaniel Smith
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef DEMO_H
#define DEMO_H

/**
 * @brief This function is for the demo of the project to show an example of being able to use uart for something.
 * 
 * @param input The character to add to the command.
 */
void uart_demo(char input);

/**
 * @brief This function is to demo schedule switching connected to the timer interrupts.
 */
void sched_demo();

#endif