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

    if (gpu_init() == 0)
        printf("GPU initialised\n");

    uint32_t hi = get_serialnr_hi((struct atag *) atags);
    uint32_t lo = get_serialnr_lo((struct atag *) atags);

    act_blink(hi + 1);

    int c;
    while ((c = getc()) != 0x4) {
        putc(c);
    }

    puts("\nGoodbye!");

}
