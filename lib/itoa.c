/*
 * Copyright (c) 2026 Nathaniel Smith
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "kernel/stdint.h"
#include "kernel/stdbool.h"

#define INT_LENGTH 10

char *itoa(int value, char *str, int base)
{
	str[0] = '\0';
	str[1] = '\0';
	bool negative = (value < 0);

	unsigned int unsigned_value = value;
	if (negative)
	{
		unsigned_value = -unsigned_value;
	}
	while (unsigned_value != 0)
	{
		uint8_t single_digit = (unsigned_value % base) + '0';
		if (single_digit > '9')
		{
			single_digit += 'a' - '9' - 1;
		}
		unsigned_value /= base;
		int i;
		for (i = (negative ? 1 : 0); str[i] != '\0'; i++)
		{
			char temp_char = str[i];
			str[i] = single_digit;
			single_digit = temp_char;
		}
		str[i] = single_digit;
	}
	if (str[0] == '\0')
	{
		str[0] = '0';
	}
	if (negative)
	{
		str[0] = '-';
	}
	return str;
}