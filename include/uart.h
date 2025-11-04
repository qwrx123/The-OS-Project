#ifndef UART_H
#define UART_H

//UART hardcoded addresses
//#define UARTADDRESS 0x2860000 //Address for the ns16550 uart chip of pocketbeagle
#define UARTADDRESS 0x09000000 //Address for the PL011 chip of qemu

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
} uart_regs_t;

//Whenever UART is full and you cannot write to it this is the flag
#define UARTFLAGFULL (1u << 5)

/**
 * @brief Initialize UART with specific hardware address (or NULL for default).
 * 
 * @param uart_device The UART hardware address to be initalized (or NULL for default).
 */
void uart_init(uart_regs_t *uart_device);

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
void uart_puts(char *s);

#endif