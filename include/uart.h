/*
 * Copyright (c) 2026 Nathaniel Smith
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef UART_H
#define UART_H

//UART hardcoded addresses
#ifdef QEMU
//Address for the PL011 chip of qemu
#define UARTADDRESS 0x09000000
#else
//Address for the ns16550 uart chip of pocketbeagle
#define UARTADDRESS 0x2860000
#endif

/**
 * @brief This struct is to define the registers inside of the
 * PL011 UART device
 */
typedef struct
{
	volatile unsigned int DR;
	volatile unsigned int RSR_ECR;
	volatile unsigned int reserved1[4];
	volatile const unsigned int FR;
	volatile unsigned int reserved2;
	volatile unsigned int ILPR;
	volatile unsigned int IBRD;
	volatile unsigned int FBRD;
	volatile unsigned int LCR_H;
	volatile unsigned int CR;
	volatile unsigned int IFLS;
	volatile unsigned int IMSC;
	volatile const unsigned int RIS;
	volatile const unsigned int MIS;
	volatile unsigned int ICR;
	volatile unsigned int DMACR;
	volatile unsigned int reserved3[997];
	volatile const unsigned int PeriphID0;
	volatile const unsigned int PeriphID1;
	volatile const unsigned int PeriphID2;
	volatile const unsigned int PeriphID3;
	volatile const unsigned int PCellID0;
	volatile const unsigned int PCellID1;
	volatile const unsigned int PCellID2;
	volatile const unsigned int PCellID3;
} uart_regs_pl011_t;

/**
 * @brief This struct is to define the registers inside of the
 * 16550 UART device
 */
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
} uart_regs_16550_t;

/**
 * @brief This enum is to differentiate between the types of UART used
 */
typedef enum
{
	id_default,
	id_pl011,
	id_16550
} uart_type;

/**
 * @brief This union is used to store the register pattern of the uart
 * being used at any one time.
 */
typedef union
{
	uart_regs_pl011_t id_pl011;
	uart_regs_16550_t id_16550;
} uart_regs_t;

//Whenever UART is full and you cannot write to it this is the flag
#define UARTPL011FLAGFULL (1u << 5)
#define UART16550FLAGFULL (1u << 5)

//Stuff for interrupts
#define PL011_RXIM (1u << 4)
#define PL011_ICR_CLEAR 0x7ff
#define NS16550_IER_RDA (1u << 0)

/**
 * @brief Initialize UART with specific hardware address (or NULL for default).
 * 
 * @param uart_device The UART hardware address to be initalized (or NULL for default).
 */
void uart_init(uart_regs_t *uart_device, uart_type init_type);

/**
 * @brief This function writes a single character to the UART address once the UART Full flag is Unset
 * 
 * @param c The character to be written
 */
void uart_putc(char c);

/**
 * @brief This function writes a whole string to the UART device, must be NULL terminated
 * 
 * @param s String to write to the UART device
 */
void uart_puts(const char *s);

#endif