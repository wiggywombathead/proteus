#ifndef _USB_DEV_H
#define _USB_DEV_H

#include <usb_descriptors.h>

#define MAX_CHILDREN_PER_DEV 10
#define MAX_INTERFACES_PER_DEV 8
#define MAX_ENDPOINTS_PER_DEV 16

/**
 * status of USB device
 * 9.1
 */
enum usbdev_status {
    ATTACHED = 0,
    POWERED = 1,
    DEFAULT = 2,
    ADDRESSED = 3,
    CONFIGURED = 4,
};

#endif
