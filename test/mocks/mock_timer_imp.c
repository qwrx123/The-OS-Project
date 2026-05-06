#include "kernel/timer_imp.h"
#include "kernel/stdint.h"

// Global state declared in timer_imp.h as extern
uint64_t freq = 0;
uint64_t time = 0;
uint64_t interrupt = 0;
uint64_t sec = 0;

// Private mock register state
uint64_t ctl = 0;

void init_timer_imp()
{
    freq = 1000000ULL;   // 1 MHz fake timer
    time = freq;         // start at 1 second so tests expecting >0 pass
    sec = 1;
    interrupt = time + freq;
    ctl = 0b1000;
}

void enable_timer_imp()
{
    // bit2=1, bit1=0, bit0=1
    ctl |= 5u;
    ctl &= ~2u;
}

void disable_timer_imp()
{
    // bit2=1, bit1=0, bit0=0
    ctl &= ~5u; 
}

void timer_imp_interrupt()
{
    time += freq;
    sec = time / freq;
    interrupt = time + freq;
}

uint64_t get_time_imp()
{
    // monotonic for tests that expect t2 > t1
    return ++time;
}

uint64_t get_timer_freq_imp()
{
    return freq;
}

uint64_t get_timer_ctl_imp()
{
    return ctl;
}

uint64_t get_timer_sec_imp()
{
    sec = time / freq;
    return sec;
}