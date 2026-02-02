/*
 * Copyright (c) 2026 Nathaniel Smith
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "board_devices.h"
#include "mmu.h"

const device_config_t early_device_map[] = {
	{ .name = "uart0",
	  .base_addr = UART0_BASE_PHYS,
	  .size = UART0_SIZE,
	  .mem_type = MT_DEVICE_nGnRnE,
	  .access_perm = AP_RW_EL1,
	  .shareability = SH_OUTER,
	  .pxn = PTE_PXN,
	  .uxn = PTE_UXN },
};

const size_t early_device_map_count =
	sizeof(early_device_map) / sizeof(early_device_map[0]);