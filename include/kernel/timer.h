#ifndef _TIMER_H
#define _TIMER_H

#include <stdint.h>
#include <kernel/peripheral.h>

#define SYSTIMER_BASE (PERIPHERAL_BASE + SYSTIMER_OFFSET)

/* system timer control/status register */
struct timer_ctrl {
    uint8_t matched0 : 1;
    uint8_t matched1 : 1;
    uint8_t matched2 : 1;
    uint8_t matched3 : 1;
    uint32_t reserved : 28;
};

/* system timer peripheral register */
struct sys_timer {
    struct timer_ctrl control;
    uint32_t counter_low;
    uint32_t counter_high;
    uint32_t compare0;
    uint32_t compare1;
    uint32_t compare2;
    uint32_t compare3;
};

void timer_init(void);
void timer_set(uint32_t);
void uwait(uint32_t);
void udelay(uint32_t);

#endif
