#include <kernel/timer.h>
#include <kernel/interrupt.h>
#include <common/stdio.h>

static struct sys_timer *systimer;

static void timer_irq_handler(void) {
    printf("Time, gentlemen!\n");
    timer_set(3000000);
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

void wait_us(uint32_t usecs) {
    volatile uint32_t ts = systimer->counter_low;

    while ((systimer->counter_low - ts) < usecs)
        ;
}
