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
enum usb_dev_status {
    USB_DEV_ATTACHED   = 0,
    USB_DEV_POWERED    = 1,
    USB_DEV_DEFAULT    = 2,
    USB_DEV_ADDRESSED  = 3,
    USB_DEV_CONFIGURED = 4,
};

/* status of last USB transfer */
enum usb_transfer_err {
    TRANSFER_OK         = 0,
    TRANSFER_STALL      = 1 << 1,
    TRANSFER_BUFF_ERR   = 1 << 2,
    TRANSFER_BABBLE     = 1 << 3,
    TRANSFER_NO_ACK     = 1 << 4,
    TRANSFER_CRC_ERR    = 1 << 5,
    TRANSFER_BIT_ERR    = 1 << 6,
    TRANSFER_CONN_ERR   = 1 << 7,
    TRANSFER_AHB_ERR    = 1 << 8,
    TRANSFER_NOT_YET    = 1 << 9,
    TRANSFER_PROCESSING = 1 << 31
};

/* start of a device specific data field */
struct usb_driver_data_header {
    uint32_t dev_driver;    // uniquely identifies driver that set data field
    uint32_t data_size;     // size in bytes of dev specific data field
};

/* store details of detected USB device */
struct usb_device {
    uint32_t number;
    usbspeed_t speed;
    enum usb_dev_status status;
    volatile uint8_t config_index;
    uint8_t portno;
    volatile enum usb_transfer_err error __attribute__((aligned(4)));
    void (*dev_detach)(struct usb_device *dev) __attribute__((aligned(4)));
    void (*dev_dealloc)(struct usb_device *dev);
    void (*dev_check_change)(struct usb_device *dev);
    void (*dev_detach_child)(struct usb_device *dev, struct usb_device *child);
    devcall (*dev_reset_child)(struct usb_device *dev, struct usb_device *child); 
    devcall (*dev_check_conn)(struct usb_device *dev, struct usb_device *child);
    volatile struct usb_dev_desc descriptor __attribute__((aligned(4)));
    volatile struct usb_config_desc config __attribute__((aligned(4)));
    volatile struct usb_interface_desc interfaces[MAX_INTERFACES_PER_DEV] __attribute__((aligned(4)));
    volatile struct usb_endpoint_desc endpoints[MAX_INTERFACES_PER_DEV][MAX_ENDPOINTS_PER_DEV] __attribute((aligned(4)));
    struct usb_device *parent __attribute__((aligned(4)));
    volatile void *full_config;
    volatile struct usb_driver_data_header *driver_data;
    volatile uint32_t last_transfer;
};

// TODO

#endif
