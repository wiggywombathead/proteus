#include <stddef.h>
#include <kernel/gpio.h>
#include <kernel/timer.h>
#include <common/stdio.h>

void mmio_write(uint32_t reg, uint32_t data) {
    *(volatile uint32_t *) reg = data;
}

uint32_t mmio_read(uint32_t reg) {
    return *(volatile uint32_t *) reg;
}

/* Loop in a way that compiler won't optimize */
static inline void delay(int32_t count) {
    asm volatile(
            "__delay_%=: subs %[count], %[count], #1; bne __delay_%=\n" :
            "=r"(count): [count]"0"(count) : "cc"
    );
}

void uart_init() {

    mmio_write(UART0_CR, 0x00000000);

    mmio_write(GPPUD, 0x00000000);
    delay(150);
    mmio_write(GPPUDCLK0, (1 << 14) | (1 << 15));
    delay(150);
    mmio_write(GPPUDCLK0, 0x00000000);

    mmio_write(UART0_ICR, 0x7ff);
    mmio_write(UART0_IBRD, 1);
    mmio_write(UART0_FBRD, 40);

    mmio_write(UART0_LCRH, (1 << 4) | (1 << 5) | (1 << 6));

    mmio_write(UART0_IMSC, (1 << 1) | (1 << 4) | (1 << 5) | (1 << 6) |
            (1 << 7) | (1 << 8) | (1 << 9) | (1 << 10));

    mmio_write(UART0_CR, (1 << 0) | (1 << 8) | (1 << 9));
}

void uart_putc(unsigned char c) {
    while (mmio_read(UART0_FR) & (1 << 5))
        ;
    mmio_write(UART0_DR, c);
}

unsigned char uart_getc() {
    while (mmio_read(UART0_FR) & (1 << 4))
        ;
    return mmio_read(UART0_DR);
}

void uart_puts(const char* str) {
    for (size_t i = 0; str[i] != '\0'; i++)
        uart_putc((unsigned char) str[i]);
}

void act_init(void) {
    mmio_write(ACT_GPFSEL, 1 << ACT_GPFBIT);
}

void act_on(void) {
    mmio_write(ACT_GPSET, 1 << ACT_GPBIT);
}

void act_off(void) {
    mmio_write(ACT_GPCLR, 1 << ACT_GPBIT);
}

void act_blink(uint32_t n) {
    while (n--) {
        act_on();
        uwait(200000);
        act_off();
        uwait(200000);
    };
}
