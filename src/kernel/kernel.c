#include <kernel/atag.h>
#include <kernel/gpio.h>
#include <kernel/gpu.h>
#include <kernel/interrupt.h>
#include <kernel/memory.h>
#include <kernel/mutex.h>
#include <kernel/proc.h>
#include <kernel/timer.h>

#include <common/stdlib.h>
#include <common/stdio.h>

mutex_t mutex;

void test(void) {
    int i = 0;
    while (1) {
        if (i % 10 == 0)
            mutex_lock(&mutex);
        else if (i % 10 == 9)
            mutex_unlock(&mutex);

        printf("TEST: %d\n", i++);
        uwait(2000000);
    }
}

void fib(void) {
    int32_t a, b, c;
    a = 0;
    b = 1;

    uint32_t i = 3;

    while (1) {
        c = a + b;
        a = b;
        b = c;

        // overflowed - start again
        if (c < 0) {
            a = 0;
            b = 1;
            c = 1;
            i = 3;
        }

        printf("Fib[%d]: %d\n", i++, c);
        uwait(1000000);
    }
}

void flash(void) {
    uint32_t hertz = 5;
    while (1) {
        act_on();
        uwait(500000 / hertz);
        act_off();
        uwait(500000 / hertz);
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
            "*  Welcome to PLACEHOLDEROS v0.001  *\n"
            "=====================================\n"
            "\n"
        );

    int mem = get_total_mem((struct atag *) atags) / (1024 * 1024);
    printf("Memory: %dMB\n", mem);

    /*
     * SUCCESSFULLY INITIALISED
     */

    mutex_init(&mutex);

    create_kthread(test, "Test", 5);
    create_kthread(fib, "fib", 4);
    create_kthread(flash, "act_flash", 10);

    uint32_t max = UINT32_MAX;
    printf("MAX: %d\n", max);

    int i = 0;
    while (1) {

        if (i % 10 == 0)
            mutex_lock(&mutex);
        else if (i % 10 == 9)
            mutex_unlock(&mutex);

        printf("main: %d\n", i++);

        uwait(2000000);
    }

    int c;
    while ((c = getc()) != 0x4) {
        putc(c);
    }

    puts("\nGoodbye!");

}
