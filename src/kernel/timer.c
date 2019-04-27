#include <kernel/timer.h>
#include <kernel/interrupt.h>
#include <kernel/sched.h>
#include <kernel/attrib.h>
#include <common/stdio.h>

static struct sys_timer *systimer;

static uint32_t delay;

/**
 * Handler for the timer IRQ - this is called each time the timer reaches 0
 */
static void timer_irq_handler(void) {
    schedule();

    uptime += delay;
    delay = 0;
}

static void timer_irq_clearer(void) {
    systimer->control.matched1 = 1;
}

/**
 * Initialise the system timer - this registers the timer_irq_handler function
 * as the system timer IRQ's handler function
 */
void timer_init(void) {
    systimer = (struct sys_timer *) SYSTIMER_BASE;
    register_irq_handler(SYS_TIMER1, timer_irq_handler, timer_irq_clearer);
}

/**
 * Set the timer to go off in some number of microseconds
 * @param usecs Microseconds to wait before firing the next IRQ
 */
void timer_set(uint32_t usecs) {
    systimer->compare1 = systimer->counter_low + usecs;
    delay = usecs;
}

/**
 * Do nothing for some number of microseconds
 * @param usecs Microseconds for which to do nothing
 */
void OPTIMIZE(0) uwait(uint32_t usecs) {
    volatile uint32_t curr = systimer->counter_low;
    volatile uint32_t t = systimer->counter_low - curr;
    while (t < usecs) {
        t = systimer->counter_low - curr;
    }
}
