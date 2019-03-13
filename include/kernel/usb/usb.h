#ifndef _USB_H
#define _USB_H

#include <kernel/usb/types.h>
#include <kernel/usb/device.h>
#include <kernel/usb/device_request.h>
#include <kernel/usb/usb_pipe.h>

devcall usb_init(void);

/* get the descriptor for a given device */
devcall usb_get_desc(struct usb_device *dev, enum usb_desc_type type,
    uint8_t index, uint16_t lang, void *buf, uint32_t length,
    uint32_t min_length, uint8_t recipient);

/* send control message synchronously to a device */
devcall usb_send_ctrl_msg(struct usb_device *dev, struct usb_pipe_addr pipe,
    void *buf, uint32_t buflen, struct usb_device_request *request,
    uint32_t timeout);

/* allocate memory to new device */
devcall usb_dev_alloc(struct usb_device **dev);

/* deallocate memory and resources of device */
void usb_dev_free(struct usb_device *dev);

/* recursively enumerate a new device
 * assigns address, determines nature of device, and configure the device
 * to recursively look for new devices (for hub) or with default config
 */
devcall usb_dev_attach(struct usb_device *dev);

/* return description of a device */
const char *usb_get_description(struct usb_device *dev);

/* return pointer to root hub device */
struct usb_device *get_root_hub(void);

/* scan entire USB tree for changes */
void usb_check_change(void);

#endif
