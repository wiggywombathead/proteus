#include <kernel/atag.h>
#include <kernel/debug.h>
#include <kernel/gpio.h>
#include <kernel/gpu.h>
#include <kernel/interrupt.h>
#include <kernel/memory.h>
#include <kernel/mmu.h>
#include <kernel/mutex.h>
#include <kernel/proc.h>
#include <kernel/sched.h>
#include <kernel/timer.h>

#include <common/stdlib.h>
#include <common/stdio.h>

mutex_t mutex;

void sys_init(uint32_t r0, uint32_t r1, uint32_t atags);

void flash(void) {
    uint32_t hertz = 5;
    while (1) {
        act_on();
        uwait(500000 / hertz);
        act_off();
        uwait(500000 / hertz);
    }
}

void fib(void) {
    int a, b, c;

    a = 0, b = 1;
    int i = 2;
    printf("[0]: 0\n[1]: 1\n");
    while (i < 20) {
        c = a + b;
        a = b;
        b = c;
        i++;
        printf("[%d]: %d\n", i, c);
        uwait(1000000);
    }
    puts("Done!\n");
}

void print1(void) {
    mutex_lock(&mutex);
    for (int i = 0; i < 100; i++) {
        putc('*');
        uwait(100000);
    }
    putc('\n');
    mutex_unlock(&mutex);
}

void print2(void) {
    mutex_lock(&mutex);
    for (int i = 0; i < 100; i++) {
        putc('o');
        uwait(100000);
    }
    putc('\n');
    mutex_unlock(&mutex);
}

void test(void) {
    int i = 0;
    while (1) {
        printf("test %d\n", i++);
        uwait(1000000);
    }
}

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
    uwait(5000000);

    mutex_init(&mutex);

    mmio_write(0x00045678, 0x00045678);
    mmio_write(0x00145678, 0x00145678);
    mmio_write(0x00245678, 0x00245678);
    mmio_write(0x00345678, 0x00345678);

    printf("0x%x...\n0x%x...\n0x%x...\n0x%x...\n",
            mmio_read(0x00045678),
            mmio_read(0x00145678),
            mmio_read(0x00245678),
            mmio_read(0x00345678)
          );

    create_kthread(fib, "fib", 4);
    
    uint32_t i = 0;
    while (1) {
        printf("main : %d\n", i++);
        uwait(1000000);
    }

    puts("\nGoodbye!");

}

void sys_init(uint32_t r0, uint32_t r1, uint32_t atags) {

    (void) r0;
    (void) r1;

    gpu_init();
    printf("Initialising GPU... DONE\n");

    printf("Initialising interrupts... ");
    interrupts_init();
    printf("DONE\n");

    printf("Initialising MMU... ");
    mmu_init();
    printf("DONE");

    printf("Initialising memory... ");
    mem_init((struct atag *) atags);
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
