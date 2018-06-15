#include <kernel/uart.h>

void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags) {

    (void) r0;
    (void) r1;
    (void) atags;

    uart_init();
    uart_puts("Hello, kernel world!\r\n");

    int c;
    while ((c = uart_getc()) != 'q') {
        uart_putc(c);
    }

    uart_puts("Goodbye!\n");
}
