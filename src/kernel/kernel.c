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
    int gpu_good = gpu_init();

    printf("Feelin' fine.\n");
    
    /* test this worked */
    if (gpu_good == 0) {
        act_blink(3);
        printf("GPU initialised well\n");
    }

    int c;
    while ((c = getc()) != 0x4) {
        putc(c);
    }

    puts("Goodbye!");

}
