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
#include <kernel/shm.h>
#include <kernel/timer.h>

#include <common/stdlib.h>
#include <common/stdio.h>

mutex_t mutex;

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
    mutex_lock(&mutex);
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
    mutex_unlock(&mutex);
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

struct shm_section *shared;

void producer(void) {
    shared = shm_open("SHARED");

    int data = 69420;
    shm_write(shared, &data, sizeof(int));
}

void consumer(void) {
    void *data = shm_read(shared, "SHARED");
    printf("SHARED: %s\n", * (int *) data);
}

void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags) {

    gpu_init();
    puts("Initialising GPU... DONE");

    printf("Initialising interrupts... ");
    interrupts_init();
    puts("DONE");

    printf("Initialising MMU...");
    for (uint32_t i = 0x0; ; i += 0x00100000) {
        mmu_section(i, i, 0x0000);
        if (i == 0xfff00000) break;
    }

    mmio_write(0x00145678, 0x00000001);
    mmio_write(0x00245678, 0x00000002);
    mmio_write(0x00345678, 0x00000003);
    mmio_write(0x00445678, 0x00000004);

    mmu_section(0x00100000, 0x00100000, 0x0000);
    mmu_section(0x00200000, 0x00200000, 0x0000);
    mmu_section(0x00300000, 0x00400000, 0x0000);
    mmu_section(0x00400000, 0x00300000, 0x0000);
    tlb_invalidate();

    mmu_start(MMU_TTABLE_BASE, 0x1000|0x4|0x1);
    puts("DONE");

    printf("Initialising memory... ");
    mem_init((struct atag *) atags);
    puts("DONE");

    printf("Initialising system timer... ");
    timer_init();
    puts("DONE");

    printf("Enabling GPIO... ");
    act_init();
    puts("DONE");

    printf("Initialising processes... ");
    proc_init();
    sched_init();
    puts("DONE");

    printf("Initialising keyboard... ");
    // TODO usb_init();
    printf("DONE\n");

    /*
     * SUCCESSFULLY INITIALISED
     */
    act_blink(3);
    printf("\n"
            "=====================================\n"
            "*      Welcome to proteus v0.1      *\n"
            "=====================================\n"
            "\n"
        );
    uwait(5000000);

    mutex_init(&mutex);

    hexstring(mmio_read(0x00145678));
    hexstring(mmio_read(0x00245678));
    hexstring(mmio_read(0x00345678));
    hexstring(mmio_read(0x00445678));

    // create_kthread(print1, "p1", 4);
    // create_kthread(print2, "p2", 4);
    // create_kthread(fib, "fib", 4);
    
    create_kthread(producer, "prod", 5);
    create_kthread(consumer, "cons", 5);
    
    uint32_t i = 0;
    while (1) {
        ;
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
