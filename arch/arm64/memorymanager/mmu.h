/*
 * Copyright (c) 2026 Nathaniel Smith
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef MMU_H
#define MMU_H

/**
 * Values written to the MAIR register
 * that configures the memory type
 * 
 * DEVICE is made for memory mapped to not ram
 * NORMAL is memory in ram
 * 
 * G is gathering
 * R is reordering
 * E is Early write acknowlegement
 * 
 * NC is No Cache
 */
#define MAIR_ATTR_DEVICE_nGnRnE 0x00
#define MAIR_ATTR_DEVICE_nGnRE 0x04
#define MAIR_ATTR_NORMAL_NC 0x44
#define MAIR_ATTR_NORMAL 0xff

/**
 * Indexes into the MAIR register mapping
 * sections of memory to the corisponding type
 */
#define MT_NORMAL 0
#define MT_NORMAL_NC 2
#define MT_DEVICE_nGnRnE 3
#define MT_DEVICE_nGnRE 4

/**
 * Access permissions for page or block descriptors
 * 
 * PTE_AP_SHIFT bits to shift to get to the access permissions
 * 
 * RW is Read Write
 * 
 * RO is Read Only
 */
#define PTE_AP_SHIFT 6
#define AP_RW_EL1 (0ULL << 6)
#define AP_RW_EL0 (1ULL << 6)
#define AP_RO_EL1 (2ULL << 6)
#define AP_RO_EL0 (3ULL << 6)

/**
 * Memory is shared between processes and devices
 * 
 * NONE is no sharing of memory
 * OUTER is sharing from the cpu and system-level components
 * INNER is sharing within the cpu and across cores
 */
#define PTE_SH_SHIFT 8
#define SH_NONE (0ULL << 8)
#define SH_OUTER (2ULL << 8)
#define SH_INNER (3ULL << 8)

/**
 * Privileged program (PXN) or Unprivileged program (UXN) 
 * is able to execute a section of memory as code
 */
#define PTE_PXN (1ULL << 53)
#define PTE_UXN (1ULL << 54)

/**
 * @brief This function is called early in the boot sequence
 *  to initalize the MMU and map the memory of the kernel
 */
void early_mmu_init();

#endif