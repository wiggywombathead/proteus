#include <kernel/timer.h>
#include <kernel/interrupt.h>
#include <common/stdio.h>

static struct sys_timer *timer_registers;

static void timer_irq_handler(void) {
    printf("Time, gentlemen!\n");
    timer_set(3000000);
}

static void timer_irq_clearer(void) {
    timer_registers->control.matched1 = 1;
}

void timer_init(void) {
    timer_registers = (struct sys_timer *) SYSTIMER_BASE;
    register_irq_handler(SYS_TIMER, timer_irq_handler, timer_irq_clearer);
}

void timer_set(uint32_t usecs) {
    timer_registers->compare1 = timer_registers->counter_low + usecs;
}
