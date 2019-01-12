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
    act_blink(3);

    gpu_init();
    printf("GPU initialised\n");

    mem_init((struct atag *) atags);
    printf("Memory initialised\n");

    int mem = get_total_mem(atags) / (1024 * 1024);
    int hi, lo;
    hi = get_serial_hi(atags);
    lo = get_serial_lo(atags);

    printf("Welcome to proteus v0.1\n");
    printf("Board: %d.%d\n", hi, lo);
    printf("Memory: %dMB\n", mem);

    int c;
    while ((c = getc()) != 0x4) {
        putc(c);
    }

    puts("\nGoodbye!");

}
