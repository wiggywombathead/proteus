#include <kernel/atag.h>
#include <kernel/memory.h>
#include <kernel/gpio.h>
#include <kernel/gpu.h>
#include <common/stdlib.h>
#include <common/stdio.h>

#include <kernel/debug.h>

void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags) {

    (void) r0;
    (void) r1;
    (void) atags;

    mem_init((struct atag *) atags);
    gpu_init();

    printf("Hello world\n");

    int c;
    while ((c = getc()) != 0x4) {
        putc(c);
    }

    puts("\nGoodbye!");

}
