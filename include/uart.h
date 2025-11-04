#ifndef UART_H
#define UART_H

//UART hardcoded addresses
//#define UARTADDRESS 0x2860000 //Address for the ns16550 uart chip of pocketbeagle
#define UARTADDRESS 0x09000000 //Address for the PL011 chip of qemu
#define UARTDR (*(volatile unsigned int *)(UARTADDRESS))
#define UARTRSR (*(volatile unsigned int *)(UARTADDRESS + 0x4))
#define UARTECR (*(volatile unsigned int *)(UARTADDRESS + 0x4))
#define UARTFR (*(volatile unsigned int *)(UARTADDRESS + 0x18))
#define UARTILPR (*(volatile unsigned int *)(UARTADDRESS + 0x20))
#define UARTIBRD (*(volatile unsigned int *)(UARTADDRESS + 0x24))
#define UARTFBRD (*(volatile unsigned int *)(UARTADDRESS + 0x28))
#define UARTLCR_H (*(volatile unsigned int *)(UARTADDRESS + 0x2C))
#define UARTCR (*(volatile unsigned int *)(UARTADDRESS + 0x30))
#define UARTIFLS (*(volatile unsigned int *)(UARTADDRESS + 0x34))
#define UARTIMSC (*(volatile unsigned int *)(UARTADDRESS + 0x38))
#define UARTRIS (*(volatile unsigned int *)(UARTADDRESS + 0x3C))
#define UARTMIS (*(volatile unsigned int *)(UARTADDRESS + 0x40))
#define UARTICR (*(volatile unsigned int *)(UARTADDRESS + 0x44))
#define UARTDMACR (*(volatile unsigned int *)(UARTADDRESS + 0x48))
#define UARTPeriphID0 (*(volatile unsigned int *)(UARTADDRESS + 0xFE0))
#define UARTPeriphID1 (*(volatile unsigned int *)(UARTADDRESS + 0xFE4))
#define UARTPeriphID2 (*(volatile unsigned int *)(UARTADDRESS + 0xFE8))
#define UARTPeriphID3 (*(volatile unsigned int *)(UARTADDRESS + 0xFEC))
#define UARTPCellID0 (*(volatile unsigned int *)(UARTADDRESS + 0xFF0))
#define UARTPCellID1 (*(volatile unsigned int *)(UARTADDRESS + 0xFF4))
#define UARTPCellID2 (*(volatile unsigned int *)(UARTADDRESS + 0xFF8))
#define UARTPCellID3 (*(volatile unsigned int *)(UARTADDRESS + 0xFFC))

//Whenever UART is full and you cannot write to it this is the flag
#define UARTFLAGFULL (1u << 5)

void uart_putc(char c);
void uart_puts(char *s);

#endif