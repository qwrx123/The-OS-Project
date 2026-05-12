#include <stdint.h>

extern void uart_puts(const char *s);

static const char *exception_class_name(uint64_t esr)
{
    uint64_t ec = (esr >> 26) & 0x3f;

    switch (ec) {
        case 0x00: return "Unknown reason";
        case 0x01: return "Trapped WFI/WFE";
        case 0x0E: return "Illegal execution state";
        case 0x15: return "SVC";
        case 0x20: return "Instruction Abort, lower EL";
        case 0x21: return "Instruction Abort, same EL";
        case 0x22: return "PC Alignment Fault";
        case 0x24: return "Data Abort, lower EL";
        case 0x25: return "Data Abort, same EL";
        case 0x2F: return "SError";
        case 0x3C: return "BRK instruction";
        default:   return "Unhandled EC";
    }
}

void el1_sync_handler()
{
    uart_puts("el1_sync\n");
}

