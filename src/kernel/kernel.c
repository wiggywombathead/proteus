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
    printf("Interrupts initialised\n");

    timer_init();
    timer_set(3000000);
    printf("System Timer initialised\n");

    act_init();

    printf("Welcome to proteus v0.1\n");

    int mem = get_total_mem((struct atag *) atags) / (1024 * 1024);
    printf("Memory: %dMB\n", mem);

    int board_hi, board_lo;
    board_hi = get_serial_hi((struct atag *) atags);
    board_lo = get_serial_lo((struct atag *) atags);
    printf("Serial no.: %d.%d\n", board_hi, board_lo);

    int c;
    while ((c = getc()) != 0x4) {
        putc(c);
    }

    puts("\nGoodbye!");

}
