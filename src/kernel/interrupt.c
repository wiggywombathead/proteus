#include <common/stdio.h>

void irq_handler(void) {
    printf("IRQ handler\n");
    while (1)
        ;
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
