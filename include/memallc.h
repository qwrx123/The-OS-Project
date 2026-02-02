/*
 * Copyright (c) [2026] Jorden Wilsey Bacso
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */
#ifndef MEMALLC_H
#define MEMALLC_H

#include "kernel/types.h"
#include "kernel/stdint.h"

/**
 * @brief Initialize the memory allocator
 * @param s The virtual address of the start of the heap
 * @param e The virtual address of the end of the heap
 * @param r The maximum range of the heap
 */
void init_memallc(uintptr_t s, uintptr_t e, uint64_t r);

 /**
 * @brief Change the bounds of the heap
 * @param s The virtual address of the start of the heap
 * @param e The virtual address of the end of the heap
 */
void set_heap_bounds(uintptr_t s, uintptr_t e);

 /**
 * @brief Set the range of the heap
 * @param r The maximum range of the heap
 */
void set_range(uint64_t r);

/**
 * @brief Set the virtual address of the end of the heap
 * @param e The virtual address of the end of the heap
 */
void set_heap_e(uintptr_t e);

/**
 * @brief Get the virtual address of the start of the heap
 * @return The virtual address of the start of the heap
 */
uintptr_t get_heap_s();

/**
 * @brief Get the virtual address of the end of the heap
 * @return The virtual address of the end of the heap
 */
uintptr_t get_heap_e();

 /**
 * @brief Get the range of the heap
 * @return The maximum range of the heap
 */
uint64_t get_range();

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