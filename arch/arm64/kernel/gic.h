/*
 * Copyright (c) 2026 Nathaniel Smith
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */
#ifndef GIC_H
#define GIC_H

#include <kernel/stdint.h>

#ifdef QEMU
#define GICD_BASE_PHYS 0x08000000ULL
#define GICR_BASE_PHYS 0x080A0000ULL
#else
#define GICD_BASE_PHYS 0x01800000ULL
#define GICR_BASE_PHYS 0x01880000ULL
#endif

void gic_init(void);
void gic_enable_irq(uint32_t irq);
void gic_disable_irq(uint32_t irq);
uint32_t gic_get_active_irq(void);
void gic_eoi(uint32_t irq);

#endif