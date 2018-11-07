#include <kernel/uart.h>
#include <kernel/memory.h>
#include <kernel/gpu.h>
#include <common/stdlib.h>
#include <common/stdio.h>

void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags) {

    (void) r0;
    (void) r1;
    (void) atags;

    mem_init((struct atag *) atags);
    gpu_init();

    printf("Feelin' fine.\n");
    printf("This is a char %c and this a per cent: %%", 'G');

    int c;
    while ((c = getc()) != 'q') {
        putc(c);
    }

    puts("Goodbye!");

}
