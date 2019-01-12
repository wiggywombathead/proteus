#include <kernel/atag.h>
#include <kernel/memory.h>
#include <kernel/gpio.h>
#include <kernel/gpu.h>
#include <common/stdlib.h>
#include <common/stdio.h>

void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags) {

    (void) r0;
    (void) r1;
    (void) atags;

    act_init();
    act_on();

    mem_init((struct atag *) atags);
    gpu_init();

    int c;
    while ((c = getc()) != 0x4) {
        putc(c);
    }

    puts("\nGoodbye!");

}
