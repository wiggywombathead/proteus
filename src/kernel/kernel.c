#include <kernel/atag.h>
#include <kernel/gpio.h>
#include <kernel/gpu.h>
#include <kernel/interrupt.h>
#include <kernel/memory.h>
#include <kernel/proc.h>
#include <kernel/timer.h>

#include <common/stdlib.h>
#include <common/stdio.h>

void function(void) {
    int i = 0;
    while (1) {
        printf("test %d\n", i++);
        uwait(1000000);
    }
}

void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags) {

    (void) r0;
    (void) r1;
    (void) atags;

    gpu_init();
    printf("GPU initialised\n");

    printf("Enabling GPIO... ");
    act_init();
    printf("DONE\n");

    printf("Initialising memory... ");
    mem_init((struct atag *) atags);
    printf("DONE\n");

    printf("Initialising interrupts... ");
    interrupts_init();
    printf("DONE\n");

    printf("Initialising system timer... ");
    timer_init();
    printf("DONE\n");

    printf("Initialising processes... ");
    proc_init();
    printf("DONE\n");

    printf("\n"
            "=====================================\n"
            "*     Welcome to proteus v0.001     *\n"
            "=====================================\n"
            "\n"
        );

    int mem = get_total_mem((struct atag *) atags) / (1024 * 1024);
    printf("Memory: %dMB\n", mem);

    create_kthread(function, "test", 4);

    int i = 0;
    while (1) {
        printf("main %d\n", i++);
        uwait(1000000);
    }

    int c;
    while ((c = getc()) != 0x4) {
        putc(c);
    }

    puts("\nGoodbye!");

}
