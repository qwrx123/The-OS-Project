/*
 * Copyright (c) 2026 Nathaniel Smith
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "demo.h"
#include "kernel/types.h"
#include "kernel/stdlib.h"
#include "uart.h"
#include "scheduler.h"
#include "kernel/timer_imp.h"

void uart_demo(char input)
{
	static char command[1028] = { 0 };
	static uint32_t command_location = 0;
	if (input == '\r')
	{
		uart_puts(command);
		uart_puts("\r\n");
		command[0] = '\0';
		command_location = 0;
	}
	else if (input == 127 /*delete key*/ && command_location > 0)
	{
		command[--command_location] = '\0';
	}
	else if (command_location < sizeof(command) - 1)
	{
		command[command_location++] = input;
		command[command_location] = '\0';
	}
}

void sched_demo()
{
	if (get_timer_sec_imp() % 8 == 0)
	{
		procSwitch();
		char p[4];
		itoa(getRunningProcess()->process->proc_ID, p, 10);
		uart_puts("scheduler: Now running ");
		uart_puts(p);
		uart_puts("\r\n");
	}
}