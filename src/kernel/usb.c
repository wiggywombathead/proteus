#include <stddef.h>
#include <kernel/usb/usb.h>

#define MAX_USB_DEVICES 32

struct usb_device *devices[MAX_USB_DEVICES];

devcall (*interface_class_attach[INTERFACE_CLASS_ATTACH_COUNT])(struct usb_device *dev, uint32_t interface_no);

/* scan bus for hubs */
// devcall usb_attach_hub(void);

struct usb_device *usb_get_hub(void) {
    return devices[0];
}

void usb_load(void) {
    for (uint32_t i = 0; i < MAX_USB_DEVICES; i++)
        devices[0] = NULL;

    for (uint32_t i = 0; i < INTERFACE_CLASS_ATTACH_COUNT; i++)
        interface_class_attach[i] = 0;
}

devcall usb_init(void) {

    devcall result = 1;

    return result;
}
