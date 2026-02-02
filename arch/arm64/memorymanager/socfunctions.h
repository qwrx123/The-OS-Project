/*
 * Copyright (c) 2026 Nathaniel Smith
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef SOCFUNCTIONS_H
#define SOCFUNCTIONS_H

/**
 * @file socfunctions.h
 * @brief This file contains device dependent code for the aarch64 archetectrue
 * to read and write to SOC registers to enable hardware features
 */

#include "kernel/types.h"

static inline void write_mair_el1(uint64_t mair)
{
	__asm__ volatile("msr MAIR_EL1, %0" ::"r"(mair) : "memory");
}
static inline uint64_t read_mair_el1()
{
	uint64_t mair;
	__asm__ volatile("mrs %0, MAIR_EL1" : "=r"(mair));
	return mair;
}

static inline void write_tcr_el1(uint64_t tcr)
{
	__asm__ volatile("msr TCR_EL1, %0" ::"r"(tcr) : "memory");
}
static inline uint64_t read_tcr_el1()
{
	uint64_t tcr;
	__asm__ volatile("mrs %0, TCR_EL1" : "=r"(tcr));
	return tcr;
}

static inline void write_ttbr0_el1(uint64_t ttbr)
{
	__asm__ volatile("msr TTBR0_EL1, %0" ::"r"(ttbr) : "memory");
}
static inline uint64_t read_ttbr0_el1()
{
	uint64_t ttbr;
	__asm__ volatile("mrs %0, TTBR0_EL1" : "=r"(ttbr));
	return ttbr;
}

static inline void write_sctlr_el1(uint64_t sctlr)
{
	__asm__ volatile("msr SCTLR_EL1, %0" ::"r"(sctlr) : "memory");
	__asm__ volatile("isb");
}

static inline uint64_t read_sctlr_el1()
{
	uint64_t sctlr;
	__asm__ volatile("mrs %0, SCTLR_EL1" : "=r"(sctlr));
	return sctlr;
}

static inline void mmu_dsb_ish()
{
	__asm__ volatile("dsb ish");
}

static inline void mmu_isb()
{
	__asm__ volatile("isb");
}

#endif