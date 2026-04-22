#include "kernel/types.h"

extern uint64_t freq;
extern uint64_t time;
extern uint64_t interrupt;
extern uint64_t sec;

/**
 * @brief Initializes the timer.
 */
void init_timer_imp();

/**
 * @brief Sets bit[2] to 1, bit[1] to 0, and bit[0] of CNTPS_CTL_EL1 to 1 to enable the timer
 */
void enable_timer_imp();

/**
 * @brief Sets bit[2] to 1, bit[1] to 0, and bit[0] of CNTPS_CTL_EL1 to 0 to disable the timer
 */
void disable_timer_imp();

/**
 * @brief Handles the timer interrupt.
 */
void timer_imp_interrupt();

/**
 * @brief Gets the current timer value.
 * @return The current timer value.
 */
uint64_t get_time_imp();

/**
 * @brief Gets the timer frequency.
 * @return The timer frequency.
 */
uint64_t get_timer_freq_imp();

/**
 * @brief Gets the timer control register value.
 * @return The timer control register value.
 */
uint64_t get_timer_ctl_imp();

/**
 * @brief Gets the time in seconds.
 * @return The time in seconds.
 */
uint64_t get_timer_sec_imp();

/**
 * @brief Displays the time in seconds.
 */
void timer_to_string_imp();
