#include <kernel/kbd.h>
#include <kernel/gpio.h>
#include <common/stdio.h>

#include <usbd/usbd.h>

static void kbd_irq_handler(char c) {
    putc(c);
}

int kbd_init(void) {

    return UsbInitialise();

    /*
    if (!USPiInitialize())
        puts("Error initialising USPi");

    if (!USPiKeyboardAvailable())
        puts("No keyboard found");

    USPiKeyboardRegisterKeyPressedHandler(kbd_irq_handler);
    */
}
