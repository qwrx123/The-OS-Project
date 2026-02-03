/*
 * Copyright (c) [2026] Jorden Wilsey Bacso
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */
#ifndef MEMALLC_H
#define MEMALLC_H

#include "kernel/types.h"
#include "kernel/stdint.h"

typedef struct memblk
{
    uintptr_t addr;
    uint64_t size;
    struct memblk* next;
} memblk_t;

/**
 * @brief Initialize the memory allocator
 * @param s The virtual address of the start of the heap
 * @param r The maximum range of the heap
 */
void init_memallc(uintptr_t s, uint64_t r);

 /**
 * @brief Set the range of the heap
 * @param r The maximum range of the heap
 */
void set_range(uint64_t r);

/**
 * @brief Get the virtual address of the start of the heap
 * @return The virtual address of the start of the heap
 */
uintptr_t get_heap_s();

 /**
 * @brief Get the range of the heap
 * @return The maximum range of the heap
 */
uint64_t get_range();

/**
 * @brief Get the list of free memory blocks
 * @return Pointer to the head of the free memory block list
 */
memblk_t* get_free_memblk();

/**
 * @brief Get the list of allocated memory blocks
 * @return Pointer to the head of the allocated memory block list
 */
memblk_t* get_allc_memblk();

/**
 * @brief Allocate a block of memory from the heap
 * @param size The size of the memory block to allocate
 */
void memallc(uint64_t size);

/**
 * @brief Deallocate all of the memory from the heap
 * @param addr The address to free the memory to
 */
void free_memallc(uintptr_t addr);

/**
 * @brief Create a new memory block
 * @param addr The starting address of the memory block
 * @param size The size of the memory block
 * @return Pointer to the newly created memory block
 */
memblk_t* new_memblk(uintptr_t addr, uint64_t size);

/**
 * @brief Allocate a memory block of given size
 * @param size The size of the memory block to allocate
 * @return Pointer to the allocated memory block
 */
memblk_t* allc_memblk(uint64_t size);

/**
 * @brief Free a memory block
 * @param addr The starting address of the memory block to free
 * @param size The size of the memory block to free
 */
void free_memblk(uintptr_t addr, uint64_t size);

#endif