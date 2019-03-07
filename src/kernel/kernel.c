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

#include <kernel/uspi.h>

void UsbInitialise(void);
void UsbCheckForChange(void);
int KeyboardCount(void); 

void usb_init(void) {
    USPiInitialize();
}

extern void kbd_update(void);
extern char kbd_getchar(void);

mutex_t mutex;

void test(void) {
    int i = 0;
    while (1) {
        if (i % 10 == 0)
            mutex_lock(&mutex);
        else if (i % 10 == 9)
            mutex_unlock(&mutex);

        printf("TEST: %d\n", i++);
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
    usb_init();
    printf("DONE\n");

    printf("\n"
            "=====================================\n"
            "*      Welcome to proteus v0.1      *\n"
            "=====================================\n"
            "\n"
        );

    act_blink(3);

    /*
     * SUCCESSFULLY INITIALISED
     */

    mutex_init(&mutex);

    create_kthread(flash, "flash", 5);

    int a = 0;
    while (1) {
        printf("%d\n", a++);
        uwait(1000000);
    }

    // if (!USPiKeyboardAvailable())
    //     printf("No keyboard!\n");

    /*
    int i = 0;
    while (1) {

        // if (i % 10 == 0)
        //     mutex_lock(&mutex);
        // else if (i % 10 == 9)
        //     mutex_unlock(&mutex);

        printf("main: %d\n", i++);

        uwait(1000000);
    }

    int c;
    while ((c = getc()) != 0x4) {
        putc(c);
    }

    puts("\nGoodbye!");
    */

}
