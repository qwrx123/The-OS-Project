/*
 * Copyright (c) 2026 Nathaniel Smith
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "demo.h"
#include "kernel/types.h"
#include "kernel/stdlib.h"
#include "uart.h"

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