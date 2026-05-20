/*
 * Copyright (c) 2026 Nathaniel Smith
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "kernel/stdint.h"

#define INT_LENGTH 10

char *itoa(int value, char *str, int base)
{
	while (value != 0)
	{
		uint8_t single_digit = (value % base) + '0';
		value /= base;
		int i;
		for (i = 0; i < INT_LENGTH && str[i] != '\0'; i++)
		{
			char temp_char = str[i];
			str[i] = single_digit;
			single_digit = temp_char;
		}
		if (i < INT_LENGTH)
		{
			str[i] = single_digit;
		}
	}
	if (str[0] == '\0')
	{
		str[0] = '0';
	}
	return str;
}