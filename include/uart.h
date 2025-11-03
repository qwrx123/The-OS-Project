#ifndef UART_H
#define UART_H

//#define UARTADDRESS 0x2860000 //Address for the ns16550 uart chip of pocketbeagle
#define UARTADDRESS 0x09000000 //Address for the PL011 chip of qemu
#define UARTDATAREG (*(volatile unsigned int *)(UARTADDRESS))
#define UARTFLAGREG (*(volatile unsigned int *)(UARTADDRESS + 0x18))
#define UARTFLAGFULL (1u << 5)

void uart_putc(char c);
void uart_puts(char *s);

#endif