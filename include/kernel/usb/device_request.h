#ifndef _USB_DEVREQUEST_H
#define _USB_DEVREQUEST_H

#include <kernel/usb/types.h>

struct usb_device_request {
    uint8_t type;
    enum usb_device_request_request {
        /* USB requests */
        GET_STATUS = 0,
        CLR_FEATURE = 1,
        SET_FEATURE = 3,
        SET_ADDR = 5,
        GET_DESC = 6,
        SET_DESC = 7,
        GET_CONFIG = 8,
        SET_CONFIG = 9,
        GET_INTERFACE = 10,
        SET_INTERFACE = 11,
        SYNC_FRAME = 12,

        /* HID requests */
        GET_REPORT = 1,
        GET_IDLE = 2,
        GET_PROTO = 3,
        SET_REPORT = 9,
        SET_IDLE = 10,
        SET_PROTO = 11,
    } request : 8;
    uint16_t value;
    uint16_t index;
    uint16_t length;
} __attribute__((__packed__));

#endif
