/*
 * Copyright (c) 2026 Nathaniel Smith
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef BOARD_DEVICES_H
#define BOARD_DEVICES_H

#include "kernel/types.h"
#include "kernel/stddef.h"

/**
 * @brief UART device address and size
 * 
 * @param UART0_BASE_PHYS Physcial address for UART
 * @param UART0_SIZE Number of bytes for UART
 */
#ifdef QEMU
#define UART0_BASE_PHYS 0x09000000ULL
#define UART0_SIZE 0x1000ULL
#else
#define UART0_BASE_PHYS 0x2860000ULL
#define UART0_SIZE 0x20ULL
#endif
/**
 * @brief This struct is used to define a device region of memory
 * 
 * @param name Name of the device
 * @param base_addr the physical base address of the device
 * @param size Number of bytes of the region
 * @param mem_type Device memory type
 * @param access_perm access permissions for the region
 * @param shareability What level of sharing is needed for the device
 * @param pxn Privaged Execute Never
 * @param uxn Unpriveleged Execute Never
 */
typedef struct
{
	const char *name;
	phys_addr_t base_addr;
	size_t size;
	uint8_t mem_type;
	uint64_t access_perm;
	uint64_t shareability;
	uint64_t pxn;
	uint64_t uxn;
} device_config_t;

/**
 * @brief A list of devices needed for early execution of the OS
 * 
 * @param early_device_map_count the number of enteries in early_device_map
 */
extern const device_config_t early_device_map[];
extern const size_t early_device_map_count;

#endif