#include <gtest/gtest.h>

extern "C" {
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

static uart_mock_regs_t UARTMOCK  = {0};
#define UARTDEFAULTRESET 100;
static int UARTRESETCYCLES = 0;
extern void (*uart_fr_callback)();
extern void (*uart_dr_callback)();

void uart_fr_callback_test() {
    if (UARTRESETCYCLES == 0)
    {
        (UARTMOCK.FR) &= 0xffffffef;
        UARTRESETCYCLES = UARTDEFAULTRESET;
        return;
    }
    UARTRESETCYCLES--;
}

void uart_dr_callback_test() {
    UARTMOCK.FR |= 0x10;
}

TEST(UART, putc_clear_flag)
{
    uart_init(reinterpret_cast<uart_regs_t*>(&UARTMOCK));
    uart_putc('a');
    ASSERT_EQ('a', UARTMOCK.DR);
}