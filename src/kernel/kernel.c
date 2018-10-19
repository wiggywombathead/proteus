#include <kernel/uart.h>
#include <kernel/memory.h>
#include <kernel/gpu.h>
#include <common/stdlib.h>

void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags) {

    (void) r0;
    (void) r1;
    (void) atags;

    mem_init((struct atag *) atags);
    gpu_init();

    puts("Hello kernel!\n");

    int c;
    while ((c = getc()) != 'q') {
        putc(c);
    }

    puts("Goodbye!");

}
