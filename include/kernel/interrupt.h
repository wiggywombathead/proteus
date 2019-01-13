#ifndef _INTERRUPT_H
#define _INTERRUPT_H

#include <kernel/peripheral.h>
#include <stdint.h>

#define NUM_IRQS 72

#define IRQ_IS_BASIC(x) (( x >= 64 ))
#define IRQ_IS_GPU1(x) (( x < 32 ))
#define IRQ_IS_GPU2(x) (( x >= 32 && x < 64 ))

#define IRQ_IS_PENDING(registers, num) \
    ((IRQ_IS_BASIC(num) && ((1 << (num - 64)) & registers->irq_basic_pending)) || \
     (IRQ_IS_GPU1(num) && ((1 << (num)) & registers->irq_gpu_pending_1)) || \
     (IRQ_IS_GPU2(num) && ((1 << (num - 32)) & registers->irq_gpu_pending_2)))

enum {
    /* interrupt register base address, 0x2000b000 */
    INTERRUPTS_BASE = PERIPHERAL_BASE + INTERRUPTS_OFFSET,

    IRQ_PENDING         = (INTERRUPTS_BASE + 0x200),
    IRQ_GPU_PENDING1    = (INTERRUPTS_BASE + 0x204),
    IRQ_GPU_PENDING2    = (INTERRUPTS_BASE + 0x208),
    FIQ_CONTROL         = (INTERRUPTS_BASE + 0x20c),
    IRQ_GPU_ENABLE1     = (INTERRUPTS_BASE + 0x210),
    IRQ_GPU_ENABLE2     = (INTERRUPTS_BASE + 0x214),
    IRQ_ENABLE          = (INTERRUPTS_BASE + 0x218),
    IRQ_GPU_DISABLE1    = (INTERRUPTS_BASE + 0x21c),
    IRQ_GPU_DISABLE2    = (INTERRUPTS_BASE + 0x220),
    IRQ_DISABLE         = (INTERRUPTS_BASE + 0x224)
};

enum irq_no {
    SYS_TIMER = 1,
    USB_CONTROLLER = 9,
    ARM_TIMER = 64
};

struct interrupt_register {
    uint32_t irq_basic_pending;
    uint32_t irq_gpu_pending_1;
    uint32_t irq_gpu_pending_2;
    uint32_t fiq_control;
    uint32_t irq_gpu_enable_1;
    uint32_t irq_gpu_enable_2;
    uint32_t irq_basic_enable;
    uint32_t irq_gpu_disable_1;
    uint32_t irq_gpu_disable_2;
    uint32_t irq_basic_disable;
};

typedef void (*interrupt_handler)(void);
typedef void (*interrupt_clearer)(void);

void interrupts_init(void);
void register_irq_handler(enum irq_no, interrupt_handler, interrupt_clearer);

__inline__ int INTERRUPTS_ENABLED(void) {
    int res;
    __asm__ __volatile__("mrs %[res], cpsr": [res] "=r" (res)::);
    return ((res >> 7) & 1) == 0;
}

__inline__ void ENABLE_INTERRUPTS(void) {
    if (!INTERRUPTS_ENABLED()) {
        __asm__ __volatile__("cpsie i");
    }
}

__inline__ void DISABLE_INTERRUPTS(void) {
    if (!INTERRUPTS_ENABLED()) {
        __asm__ __volatile__("cpsid i");
    }
}

#endif
