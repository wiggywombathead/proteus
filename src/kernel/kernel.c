#include <kernel/atag.h>
#include <kernel/debug.h>
#include <kernel/gpio.h>
#include <kernel/gpu.h>
#include <kernel/interrupt.h>
#include <kernel/kbd.h>
#include <kernel/memory.h>
#include <kernel/mmu.h>
#include <kernel/mutex.h>
#include <kernel/proc.h>
#include <kernel/sched.h>
#include <kernel/shm.h>
#include <kernel/timer.h>

#include <common/stdio.h>
#include <common/stdlib.h>
#include <common/string.h>

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

    printf("[0]: 0\n");
    uwait(1000000);

    printf("[1]: 1\n");
    uwait(1000000);

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
    for (int i = 0; i < get_console_width(); i++) {
        putc('*');
        uwait(1000000 / get_console_width());
    }
    mutex_unlock(&mutex);
}

void print2(void) {
    mutex_lock(&mutex);
    for (int i = 0; i < get_console_width(); i++) {
        putc('o');
        uwait(1000000 / get_console_width());
    }
    mutex_unlock(&mutex);
}

void test(void) {
    int i = 0;
    while (1) {
        printf("test %d\n", i++);
        uwait(1000000);
    }
}

struct data {
    int v1;
    int v2;
    char s[16];
};

void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags) {

    (void) r0;
    (void) r1;

    /* GPU */
    gpu_init();
    puts("Initialising GPU");
    printf("\tScreen: %dx%d\n",
            get_screen_width(),
            get_screen_height()
        );
    printf("\tConsole: %dx%d\n",
            get_console_width(),
            get_console_height()
        );

    /* INTERRUPTS */
    puts("Initialising interrupts");
    interrupts_init();

    /* MEMORY MANAGEMENT UNIT */
    puts("Initialising MMU");
    for (uint32_t i = 0x0; ; i += 0x00100000) {
        mmu_section(i, i, 0x0000);
        if (i == 0xfff00000) break;
    }

    mmio_write(0x00300000, 0x3);
    mmio_write(0x00400000, 0x4);
    mmio_write(0x00500000, 0x5);

    mmu_section(0x00300000, 0x00400000, 0x0000);
    mmu_section(0x00400000, 0x00300000, 0x0000);
    mmu_section(0x00500000, 0x00500000, 0x0000);
    tlb_invalidate();

    mmu_start(MMU_TTABLE_BASE, 0x1000|0x4|0x1);

    /* PAGING AND HEAP */
    puts("Initialising memory");
    memory_init((struct atag *) atags);
    printf("\tHeap: %dMiB\n", KERNEL_HEAP_SIZE / MEGABYTE);
    printf("\tMemory: %dMiB\n",
            get_total_mem((struct atag *) atags) / MEGABYTE
        );

    /* SYSTEM TIMER PERIPHERAL */
    puts("Initialising system timer");
    timer_init();

    /* GPIO */
    puts("Initialising GPIO");
    uart_init();
    printf("\tUART initialised\n");
    act_init();
    printf("\tACT enabled\n");

    /* PROCESSES */
    puts("Initialising processes");
    proc_init();
    sched_init();

    /* KEYBOARD */
    puts("Initialising keyboard");
    kbd_init();
    puts("Hello?");

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
    uwait(3000000);

    uint32_t addr = shm_open() +1;
    struct data d = {1,2,"hello"};
    shm_write(addr, &d, sizeof(struct data));

    struct data *r = (struct data *) shm_read(addr);
    printf("Read: {%d, %d, %s}\n", r->v1, r->v2, r->s);

    // hexstring(mmio_read(0x00300000));
    // hexstring(mmio_read(0x00400000));
    // hexstring(mmio_read(0x00500000));

    // mutex_init(&mutex);

    // create_kthread(print1, "p1");
    // create_kthread(print2, "p2");
    // create_kthread(fib, "fib");
    
    // create_kthread(producer, "prod");
    // create_kthread(consumer, "cons");

    while (1) {

    }
    
    puts("\nGoodbye!");

}
