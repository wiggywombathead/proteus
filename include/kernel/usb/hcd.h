#ifndef _HCD_H
#define _HCD_H

#include <kernel/usb/device.h>
#include <kernel/usb/device_request.h>
#include <kernel/usb/usb_pipe.h>
#include <kernel/usb/types.h>

/* initialise host controller driver */
devcall hcd_init(void);

devcall hcd_start(void);
devcall hcd_stop(void);

devcall hcd_uninit(void);

devcall hcd_send_ctrl_msg(struct usb_device *dev, struct usb_pipe_addr pipe, void *buf, uint32_t buflen, struct usb_device_request *request);

#endif
