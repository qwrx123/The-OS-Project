/*
 * Copyright (c) [year] [your name]
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <gtest/gtest.h>
#include <iostream>
extern "C"
{
#include "uart.h"
}

typedef struct
{
	volatile unsigned int DR;
	volatile unsigned int RSR_ECR;
	volatile unsigned int reserved1[4];
	volatile unsigned int FR;
	volatile unsigned int reserved2;
	volatile unsigned int ILPR;
	volatile unsigned int IBRD;
	volatile unsigned int FBRD;
	volatile unsigned int LCR_H;
	volatile unsigned int CR;
	volatile unsigned int IFLS;
	volatile unsigned int IMSC;
	volatile unsigned int RIS;
	volatile unsigned int MIS;
	volatile unsigned int ICR;
	volatile unsigned int DMACR;
	volatile unsigned int reserved3[997];
	volatile unsigned int PeriphID0;
	volatile unsigned int PeriphID1;
	volatile unsigned int PeriphID2;
	volatile unsigned int PeriphID3;
	volatile unsigned int PCellID0;
	volatile unsigned int PCellID1;
	volatile unsigned int PCellID2;
	volatile unsigned int PCellID3;
} uart_mock_regs_t;

static uart_mock_regs_t UARTMOCK = { 0 };
#define UARTDEFAULTRESET 100;
static int UARTRESETCYCLES = 0;
static int UARTFRREAD = 0;
static std::string console;
extern void (*uart_fr_callback)();
extern void (*uart_dr_callback)();
extern void (*uart_lsr_callback)();
extern void (*uart_tbr_callback)();

void uart_fr_callback_test()
{
	UARTFRREAD++;
	if (UARTRESETCYCLES == 0)
	{
		UARTMOCK.FR &= 0xffffffdf;
		UARTRESETCYCLES = UARTDEFAULTRESET;
		return;
	}
	if (UARTRESETCYCLES > 0)
	{
		UARTRESETCYCLES--;
	}
}

void uart_dr_callback_test()
{
	UARTMOCK.FR |= 0x20;
	console.push_back(UARTMOCK.DR);
}

class UARTPL011 : public ::testing::Test
{
    protected:
	UARTPL011()
	{
	}

	virtual ~UARTPL011()
	{
	}

	virtual void SetUp()
	{
		std::cout << "here";
		UARTMOCK = { 0 };
		uart_init(reinterpret_cast<uart_regs_t *>(&UARTMOCK), id_pl011);
		UARTRESETCYCLES = UARTDEFAULTRESET;
		UARTFRREAD = 0;
		uart_fr_callback = &uart_fr_callback_test;
		uart_dr_callback = &uart_dr_callback_test;
		console.clear();
	}

	virtual void TearDown()
	{
	}
};

TEST_F(UARTPL011, putc_clear_flag)
{
	uart_putc('a');
	ASSERT_EQ('a', UARTMOCK.DR);
}

TEST_F(UARTPL011, putc_full_flag_nohang)
{
	UARTMOCK.FR |= 0x20;
	uart_putc('a');
	ASSERT_EQ('a', UARTMOCK.DR);
	ASSERT_GT(UARTFRREAD, 0);
}

TEST_F(UARTPL011, puts_writes_string)
{
	uart_puts("Hello World");
	ASSERT_EQ(console, "Hello World");
}

TEST_F(UARTPL011, puts_skips_null)
{
	uart_puts(nullptr);
}

typedef struct
{
	volatile unsigned int RBR_TBR;
	volatile unsigned int IER;
	volatile unsigned int IIR_FCR;
	volatile unsigned int LCR;
	volatile unsigned int MCR;
	volatile unsigned int LSR;
	volatile unsigned int MSR;
	volatile unsigned int SCR;
} uart_16550_mock_regs_t;

static uart_16550_mock_regs_t UART16550MOCK = { 0 };
static int UART16550LSRREAD = 0;

void uart_16550_lsr_callback_test()
{
	UART16550LSRREAD++;
	if (UARTRESETCYCLES == 0)
	{
		UART16550MOCK.LSR |= 0x20;
		UARTRESETCYCLES = UARTDEFAULTRESET;
		return;
	}
	if (UARTRESETCYCLES > 0)
	{
		UARTRESETCYCLES--;
	}
}

void uart_16550_tbr_callback_test()
{
	UART16550MOCK.LSR &= ~0x20;
	console.push_back(UART16550MOCK.RBR_TBR);
}

class UART16550 : public ::testing::Test
{
    protected:
	UART16550()
	{
	}

	virtual ~UART16550()
	{
	}

	virtual void SetUp()
	{
		UART16550MOCK = { 0 };
		UART16550MOCK.LSR = 0x20;
		uart_init(reinterpret_cast<uart_regs_t *>(&UART16550MOCK),
			  id_16550);
		UARTRESETCYCLES = UARTDEFAULTRESET;
		UART16550LSRREAD = 0;
		console.clear();
		uart_lsr_callback = &uart_16550_lsr_callback_test;
		uart_tbr_callback = &uart_16550_tbr_callback_test;
	}

	virtual void TearDown()
	{
	}
};

TEST_F(UART16550, putc_clear_flag)
{
	uart_putc('a');
	ASSERT_EQ('a', UART16550MOCK.RBR_TBR);
}

TEST_F(UART16550, putc_empty_flag_nohang)
{
	UART16550MOCK.LSR &= ~0x20;
	uart_putc('a');
	ASSERT_EQ('a', UART16550MOCK.RBR_TBR);
}

TEST_F(UART16550, puts_writes_string)
{
	uart_puts("Hello World");
	ASSERT_EQ(console, "Hello World");
}

TEST_F(UART16550, puts_skips_null)
{
	uart_puts(nullptr);
}

TEST_F(UART16550, init_sets_fifo)
{
	ASSERT_EQ(0x01, UART16550MOCK.IIR_FCR);
}