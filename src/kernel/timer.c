#include <kernel/timer.h>
#include <kernel/interrupt.h>
#include <kernel/sched.h>

#include <common/stdio.h>

static struct sys_timer *systimer;
static int uptime;

static void timer_irq_handler(void) {
    schedule();
}

static void timer_irq_clearer(void) {
    systimer->control.matched1 = 1;
}

void timer_init(void) {
    systimer = (struct sys_timer *) SYSTIMER_BASE;
    register_irq_handler(SYS_TIMER, timer_irq_handler, timer_irq_clearer);
}

void timer_set(uint32_t usecs) {
    systimer->compare1 = systimer->counter_low + usecs;
}

void __attribute__((optimize(0))) uwait(uint32_t usecs) {
    volatile uint32_t curr = systimer->counter_low;
    volatile uint32_t t = systimer->counter_low - curr;
    while (t < usecs) {
        t = systimer->counter_low - curr;
    }
}
