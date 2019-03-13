#ifndef _USB_PIPE_H
#define _USB_PIPE_H

#include <kernel/usb/types.h>

struct usb_pipe_addr {
    usbpktsize_t max : 2;
    usbspeed_t speed : 2;
    unsigned endpoint : 4;
    unsigned device : 8;
    usbtransfer_t type : 2;
    usbdirection_t direction : 1;
    unsigned reserved : 13;  // 19-31
} __attribute__((packed));

#endif
