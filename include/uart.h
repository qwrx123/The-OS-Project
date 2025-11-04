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

#define UART ((uart_regs_t *)UARTADDRESS)

//Whenever UART is full and you cannot write to it this is the flag
#define UARTFLAGFULL (1u << 5)

void uart_putc(char c);
void uart_puts(char *s);

#endif