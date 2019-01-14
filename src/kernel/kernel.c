#include <kernel/atag.h>
#include <kernel/memory.h>
#include <kernel/gpu.h>
#include <kernel/interrupt.h>
#include <kernel/timer.h>
#include <kernel/gpio.h>

#include <common/stdlib.h>
#include <common/stdio.h>

void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags) {

    (void) r0;
    (void) r1;
    (void) atags;

    gpu_init();
    printf("GPU initialised\n");

    mem_init((struct atag *) atags);
    printf("Memory initialised\n");

    interrupts_init();
    timer_init();

    act_init();

    /*
    printf("Welcome to proteus v0.1\n");
    printf("Board: %d.%d\n", hi, lo);
    printf("Memory: %dMB\n", mem);
    */

    while (1) {
        act_on();
        wait_us(1000);
        act_off();
        wait_us(1000);

        act_blink(4);
    }

    int c;
    while ((c = getc()) != 0x4) {
        putc(c);
    }

    puts("\nGoodbye!");

}
