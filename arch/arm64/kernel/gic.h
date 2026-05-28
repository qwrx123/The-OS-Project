/*
 * Copyright (c) 2026 Nathaniel Smith
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */
#ifndef GIC_H
#define GIC_H

#include <kernel/stdint.h>

/**
 * @brief Base physical address for the GIC Distributor and Redistributer
 * as well as.
 */
#ifdef QEMU
#define GICD_BASE_PHYS 0x08000000ULL
#define GICR_BASE_PHYS 0x080A0000ULL
#define UART_IRQ ((uint32_t)33)
#define TIMER_IRQ ((uint32_t)14)
#else
#define GICD_BASE_PHYS 0x01800000ULL
#define GICR_BASE_PHYS 0x01880000ULL
#define UART_IRQ ((uint32_t)216)
#define TIMER_IRQ ((uint32_t)14)
#endif

/**
 * @brief This function initializes the gic and related devices (GIC, Distributer, Redistributer)
 * 
 */
void gic_init();

/**
 * @brief This function enables a IRQ for a specific chip.
 * 
 * @param irq ID for the irq to enable.
 */
void gic_enable_irq(uint32_t irq);

/**
 * @brief This function disables a IRQ for a specific chip.
 * 
 * @param irq ID for the irq to disable.
 */
void gic_disable_irq(uint32_t irq);

/**
 * @brief This function gets the active IRQ from the GIC and returns it.
 * 
 * @return uint32_t The ID of the active IRQ.
 */
uint32_t gic_get_active_irq();

/**
 * @brief This function signals the end of an interrupt to the GIC.
 * 
 * @param irq ID for the irq to signal the end of.
 */
void gic_eoi(uint32_t irq);

#endif