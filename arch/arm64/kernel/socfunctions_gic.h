/*
 * Copyright (c) 2026 Nathaniel Smith
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef SOCFUNCTIONS_GIC_H
#define SOCFUNCTIONS_GIC_H

#include "kernel/types.h"
#include <kernel/stdint.h>

static inline void write_icc_sre_el1(uint64_t v)
{
	__asm__ volatile("msr ICC_SRE_EL1, %0" ::"r"(v) : "memory");
}

static inline void write_icc_pmr_el1(uint64_t v)
{
	__asm__ volatile("msr ICC_PMR_EL1, %0" ::"r"(v) : "memory");
}

static inline void write_icc_igrpen1_el1(uint64_t v)
{
	__asm__ volatile("msr ICC_IGRPEN1_EL1, %0" ::"r"(v) : "memory");
}

static inline uint64_t read_icc_iar1_el1()
{
	uint64_t val;
	__asm__ volatile("mrs %0, ICC_IAR1_EL1" : "=r"(val) : : "memory");
	return val;
}

static inline void write_icc_eoir1_el1(uint64_t v)
{
	__asm__ volatile("msr ICC_EOIR1_EL1, %0" ::"r"(v) : "memory");
}

#endif /* SOCFUNCTIONS_GIC_H */