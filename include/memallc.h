/*
 * Copyright (c) [2026] Jorden Wilsey Bacso
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */
#ifndef MEMALLC_H
#define MEMALLC_H

#include "kernel/types.h"

uintptr_t heap_s = 0x1000;
uintptr_t heap_e;

uint64_t min_bytes = 0x00000000000001F0;
uint64_t range = 0x400000;

 /**
 * @brief Set the physical address of the end of the heap
 * @param e The physical address of the end of the heap
 */
void set_heap_e(phys_addr_t e);

 /**
 * @brief Get the physical address of the start of the heap
 * @return The physical address of the start of the heap
 */
uintptr_t get_heap_s();

 /**
 * @brief Get the physical address of the end of the heap
 * @return The physical address of the end of the heap
 */
uintptr_t get_heap_e();

 /**
 * @brief Allocate a block of memory from the heap
 * @param size The size of the memory block to allocate
 */
void memallc(uint64_t size);

 /**
 * @brief Deallocate all of the memory from the heap
 */
void free_memallc();

 #endif