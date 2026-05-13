/*
 * Copyright (c) 2026 Nathaniel Smith
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "board_devices.h"
#include "mmu.h"

#ifdef QEMU
#define GICD_BASE_PHYS 0x08000000ULL
#define GICR_BASE_PHYS 0x080A0000ULL
#else
#define GICD_BASE_PHYS 0x01800000ULL
#define GICR_BASE_PHYS 0x01880000ULL
#endif

const device_config_t early_device_map[] = {
	{ .name = "uart0",
	  .base_addr = UART0_BASE_PHYS,
	  .size = UART0_SIZE,
	  .mem_type = MT_DEVICE_nGnRnE,
	  .access_perm = AP_RW_EL1,
	  .shareability = SH_OUTER,
	  .pxn = PTE_PXN,
	  .uxn = PTE_UXN },
	{ .name = "gicd",
	  .base_addr = GICD_BASE_PHYS,
	  .size = 0x10000ULL,
	  .mem_type = MT_DEVICE_nGnRnE,
	  .access_perm = AP_RW_EL1,
	  .shareability = SH_OUTER,
	  .pxn = PTE_PXN,
	  .uxn = PTE_UXN },
	{ .name = "gicr",
	  .base_addr = GICR_BASE_PHYS,
	  .size = 0xC0000ULL,
	  .mem_type = MT_DEVICE_nGnRnE,
	  .access_perm = AP_RW_EL1,
	  .shareability = SH_OUTER,
	  .pxn = PTE_PXN,
	  .uxn = PTE_UXN },
};

const size_t early_device_map_count =
	sizeof(early_device_map) / sizeof(early_device_map[0]);