#include <kernel/kbd.h>
#include <kernel/gpio.h>
#include <common/stdio.h>

#include <uspi.h>
#include <uspios.h>

void kbd_irq_handler(const char *c) {
    puts(c);
}

int kbd_init(void) {

    printf("\tStarting USPi - ");
    if (!USPiInitialize())
        puts("Error initialising USPi");
    puts("DONE");

    printf("\tFinding keyboard - ");
    if (!USPiKeyboardAvailable())
        puts("not found");
    puts("found");

    USPiKeyboardRegisterKeyPressedHandler(kbd_irq_handler);
    return 0;
}
