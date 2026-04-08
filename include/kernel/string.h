/*
 * Copyright (c) 2026 Nathaniel Smith
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef STRING_H
#define STRING_H

#include "kernel/types.h"

/**
 * @brief Sets section of memory to number
 * 
 * @param dest Address of memory to set
 * @param ch Value to set memory to
 * @param count Number of bytes to set
 * @return void* Address of block set
 */
extern void *memset(void *dest, int ch, uint64_t count);

#endif