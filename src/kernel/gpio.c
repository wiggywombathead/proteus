#include <stddef.h>
#include <kernel/gpio.h>
#include <kernel/timer.h>
#include <common/stdio.h>

/**
 * Write 32-bit value to register
 * @param reg Register to write
 * @param data Value to write to the register
 */
void mmio_write(uint32_t reg, uint32_t data) {
	*(volatile uint32_t *) reg = data;
}

/**
 * Read value from register
 * @param reg Register from which to read
 */
uint32_t mmio_read(uint32_t reg) {
	return *(volatile uint32_t *) reg;
}

/**
 * Loop in a way the compiler will not optimise (inexact)
 * @param count Number of loops
 */
static inline void delay(int32_t count) {
	asm volatile(
			"__delay_%=: subs %[count], %[count], #1; bne __delay_%=\n" :
			"=r"(count): [count]"0"(count) : "cc"
		);
}

/**
 * Initialise the UART peripheral
 */
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

/**
 * Write a character to the UART data register
 * @param c Character to send
 */
void uart_putc(unsigned char c) {
	while (mmio_read(UART0_FR) & (1 << 5))
		;
	mmio_write(UART0_DR, c);
}

/**
 * Retrieve a character from the UART data register
 */
unsigned char uart_getc() {
	while (mmio_read(UART0_FR) & (1 << 4))
		;
	return mmio_read(UART0_DR);
}

/**
 * Send a string to be printed by the UART
 * @param str String to print
 */
void uart_puts(const char *str) {
	for (size_t i = 0; str[i] != '\0'; i++)
		uart_putc((unsigned char) str[i]);
}

/**
 * Initialise the ACT LED for use
 */
void act_init(void) {
	mmio_write(ACT_GPFSEL, 1 << ACT_GPFBIT);
}

/**
 * Turn the ACT LED on
 */
void act_on(void) {
	mmio_write(ACT_GPSET, 1 << ACT_GPBIT);
}

/**
 * Turn the ACT LED off
 */
void act_off(void) {
	mmio_write(ACT_GPCLR, 1 << ACT_GPBIT);
}

/**
 * Blink the ACT LED a set number of times
 * @param n Number of times to blink
 */
void act_blink(uint32_t n) {
	while (n--) {
		act_on();
		uwait(200000);
		act_off();
		uwait(200000);
	};
}
