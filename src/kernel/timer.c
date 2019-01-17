#include <kernel/timer.h>
#include <kernel/interrupt.h>
#include <common/stdio.h>

static struct sys_timer *systimer;

static int uptime = 0;

static void timer_irq_handler(void) {
    printf("%d ", uptime++);
    timer_set(1000000);
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

void uwait(uint32_t usecs) {
    volatile uint32_t t = systimer->counter_low;
    while ((systimer->counter_low - t) < usecs)
        ;
}
