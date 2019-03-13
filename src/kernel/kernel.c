#include <kernel/atag.h>
#include <kernel/gpio.h>
#include <kernel/gpu.h>
#include <kernel/interrupt.h>
#include <kernel/memory.h>
#include <kernel/mutex.h>
#include <kernel/proc.h>
#include <kernel/sched.h>
#include <kernel/timer.h>

#include <common/stdlib.h>
#include <common/stdio.h>

mutex_t mutex;

void sys_init(uint32_t r0, uint32_t r1, uint32_t atags);
void flash(void);
void test(void);

void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags) {

    sys_init(r0, r1, atags);
    act_blink(3);

    /*
     * SUCCESSFULLY INITIALISED
     */

    printf("\n"
            "=====================================\n"
            "*      Welcome to proteus v0.1      *\n"
            "=====================================\n"
            "\n"
        );


    mutex_init(&mutex);

    create_kthread(flash, "flash", 6);
    create_kthread(test, "test", 5);

    while (1) {
        printf("%ds ", uptime/1000000);
        uwait(1000000);
    }

    puts("\nGoodbye!");

}

void sys_init(uint32_t r0, uint32_t r1, uint32_t atags) {

    (void) r0;
    (void) r1;

    gpu_init();
    printf("Initialising GPU... DONE");

    printf("Initialising memory... ");
    mem_init((struct atag *) atags);
    printf("DONE\n");

    printf("Initialising interrupts... ");
    interrupts_init();
    printf("DONE\n");

    printf("Initialising system timer... ");
    timer_init();
    printf("DONE\n");

    printf("Enabling GPIO... ");
    act_init();
    printf("DONE\n");

    printf("Initialising processes... ");
    proc_init();
    sched_init();
    printf("DONE\n");

    printf("Initialising keyboard... ");
    // TODO usb_init();
    printf("DONE\n");
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

void test(void) {
    int i = 0;
    while (1) {
        if (i % 10 == 0)
            mutex_lock(&mutex);
        else if (i % 10 == 9)
            mutex_unlock(&mutex);

        uwait(1000000);
    }
}
