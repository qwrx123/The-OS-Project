/*
 * Copyright (c) 2026 Nathaniel Smith
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "kernel/string.h"
#include "kernel/types.h"

void *memset(void *dest, int ch, uint64_t count)
{
	char *realDest = (char *)dest;
	for (uint64_t i = 0; i < count; i++)
	{
		*realDest = ch;
		realDest++;
	}
	return dest;
}