#include <kernel/interrupt.h>

#include <common/stdlib.h>
#include <common/stdio.h>

/* interrupt handlers and pending-bit clearers */
static interrupt_handler handlers[NUM_IRQS];
static interrupt_clearer clearers[NUM_IRQS];

static struct interrupt_register *interrupt_regs;

extern void move_exception_vector(void);

void interrupts_init(void) {
    interrupt_regs = (struct interrupt_register *) IRQ_PENDING;

    // zero out all handlers
    bzero(handlers, sizeof(interrupt_handler) * NUM_IRQS);
    bzero(clearers, sizeof(interrupt_clearer) * NUM_IRQS);

    // disable all interrupts - enable as they are registered
    interrupt_regs->irq_basic_disable = 0xffffffff;
    interrupt_regs->irq_gpu_disable_1 = 0xffffffff;
    interrupt_regs->irq_gpu_disable_2 = 0xffffffff;

    // copy exception vector table to address 0
    move_exception_vector();
    ENABLE_INTERRUPTS();
}

void irq_handler(void) {
    printf("IRQ handler\n");

    for (int i = 0; i < NUM_IRQS; i++) {
        
        if (IRQ_IS_PENDING(interrupt_regs, i) && handlers[i] != 0) {
            clearers[i]();
            ENABLE_INTERRUPTS();
            handlers[i]();
            DISABLE_INTERRUPTS();
            
            return;
        }

    }
}

void __attribute__((interrupt("ABORT"))) reset_handler(void) {
    printf("RESET handler\n");
    while (1)
        ;
}

void __attribute__((interrupt("ABORT"))) prefetch_abort_handler(void) {
    printf("PREFETCH ABORT handler\n");
    while (1)
        ;
}

void __attribute__((interrupt("ABORT"))) data_abort_handler(void) {
    printf("DATA ABORT handler\n");
    while (1)
        ;
}
void __attribute__((interrupt("UNDEF"))) undefined_instruction_handler(void) {
    printf("UNDEFINED INSTRUCTION handler\n");
    while (1)
        ;
}

void __attribute__((interrupt("SWI"))) software_interrupt_handler(void) {
    printf("SWI handler\n");
    while (1)
        ;
}

void __attribute__((interrupt("FIQ"))) fiq_handler(void) {
    printf("RESET handler\n");
    while (1)
        ;
}

void register_irq_handler(enum irq_no num, interrupt_handler handler, interrupt_clearer clearer) {

    uint32_t irq_pos;

    if (IRQ_IS_BASIC(num)) {
        irq_pos = num - 64;
        handlers[num] = handler;
        clearers[num] = clearer;
        interrupt_regs->irq_basic_enable |= (1 << irq_pos);
    } else if (IRQ_IS_GPU1(num)) {
        irq_pos = num;
        handlers[num] = handler;
        clearers[num] = clearer;
        interrupt_regs->irq_gpu_enable_1 |= (1 << irq_pos);
    } else if (IRQ_IS_GPU2(num)) {
        irq_pos = num - 32;
        handlers[num] = handler;
        clearers[num] = clearer;
        interrupt_regs->irq_gpu_enable_2 |= (1 << irq_pos);
    } else {
        printf("Error registering IRQ handler: invalid number (%d)\n", num);
    }

}
