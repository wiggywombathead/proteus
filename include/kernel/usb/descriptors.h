#ifndef _USB_DESC_H
#define _USB_DESC_H

#include <stdint.h>
#include <stdbool.h>

/**
 * descriptor type field from header of USB descriptors
 * defined in USB 2.0 manual in 9.6
 */
enum usb_desc_type {
    DEVICE             = 1,
    CONFIG             = 2,
    STRING             = 3,
    INTERFACE          = 4,
    ENDPOINT           = 5,
    DEV_QUALIFIER      = 6,
    OTHER_SPEED_CONFIG = 7,
    INTERFACE_PWR      = 8,
    HID                = 33,    // human interface device
    HID_REPORT         = 34,
    HID_PHYS           = 35,
    HUB                = 41,
};

/**
 * USB descriptor information header
 * defined in USB 2.0 manual in 9.6
 */
struct usb_desc_header {
    uint8_t desc_length;
    enum usb_desc_type desc_type : 8;
} __attribute__((packed));

/**
 * device descriptor info
 * defined in 9.6.1
 */
enum usb_dev_class {
    DEV_INTERFACE  = 0x00,
    DEV_COMMS      = 0x02,
    DEV_HUB        = 0x09,
    DEV_DIAGNOSTIC = 0xdc,
    DEV_MISC       = 0xef,
    DEV_VEND_SPEC  = 0xff, // vendor specific
};

struct usb_dev_desc {
    uint8_t desc_length;
    enum usb_desc_type desc_type : 8;
    uint16_t usb_version;
    enum usb_dev_class class : 8;
    uint8_t subclass;
    uint8_t protocol;
    uint8_t max_pkt_size;
    uint16_t vendor_id;
    uint16_t product_id;
    uint16_t version;
    uint8_t manufacturer;
    uint8_t product;
    uint8_t serial_no;
    uint8_t config_count;
} __attribute__((packed));

/**
 * device qualifier descriptor info
 * 9.6.2
 */
struct usb_dev_qualifier_desc {
    uint8_t desc_length;
    enum usb_desc_type desc_type;
    uint16_t usb_version;
    enum usb_dev_class class : 8;
    uint8_t subclass;
    uint8_t protocol;
    uint8_t max_pkt_size;
    uint8_t config_count;
    uint8_t reserved;
} __attribute__((packed));

/**
 * configuration descriptor info
 * 9.6.3
 */
struct usb_config_desc {
    uint8_t desc_length;
    enum usb_desc_type desc_type : 8;
    uint16_t total_length;
    uint8_t interface_count;
    uint8_t config_value;
    uint8_t str_index;
    struct {
        uint8_t reserved_04 : 5;
        bool remote_wake : 1;
        bool self_powered : 1;
        uint8_t reserved_7 : 1;
    } __attribute__((packed)) attribs;
    uint8_t max_power;
} __attribute__((packed));

/**
 * other speed config descriptor
 * 9.6.4
 */
struct usb_other_speed_config_desc {
    uint8_t desc_length;
    enum usb_desc_type desc_type;
    uint16_t total_length;
    uint8_t interface_count;
    uint8_t config_value;
    uint8_t str_index;
    struct {
        uint8_t reserved_04 : 5;
        bool remote_wake : 1;
        bool self_powered : 1;
        enum {
            valid = 1
        } reserved_7 : 1;
    } __attribute__((packed)) attribs;
    uint8_t max_power;
} __attribute__((packed));

enum interface_class {
    IFACE_RESERVED   = 0x00,
    IFACE_AUDIO      = 0x01,
    IFACE_COMMS      = 0x02,
    IFACE_HID        = 0x03,
    IFACE_PHYS       = 0x05,
    IFACE_IMG        = 0x06,
    IFACE_PRINTER    = 0x07,
    IFACE_STORAGE    = 0x08, // mass storage
    IFACE_HUB        = 0x09,
    IFACE_CDC_DATA   = 0x0a,
    IFACE_SMRT_CARD  = 0x0b,
    IFACE_SECURITY   = 0x0d,
    IFACE_VIDEO      = 0x0e,
    IFACE_HEALTHCARE = 0x0f,
    IFACE_AUDIOVIS   = 0x10,
    IFACE_DIAG_DEV   = 0xdc,
    IFACE_CONTROLLER = 0xe0, // wireless controller
    IFACE_MISC       = 0xef,
    IFACE_APP_SPEC   = 0xfe, // application specific
    IFACE_VEND_SPEC  = 0xff, // vendor specific
};

/**
 * interface descriptor info
 * 9.6.5
 */
struct usb_interface_desc {
    uint8_t desc_length;
    enum usb_desc_type desc_type;
    uint8_t number;
    uint8_t alt_setting;
    uint8_t endpoint_count;
    enum interface_class class : 8;
    uint8_t subclass;
    uint8_t protocol;
    uint8_t str_index;
} __attribute__((packed));

struct usb_endpoint_addr {
    uint8_t number : 4;
    uint8_t reserved : 3;   // bits 4-6
    usbdirection_t direction : 1;
} __attribute__((packed));

/**
 * endpoint descriptor info
 * 9.6.6
 */
struct usb_endpoint_desc {
    uint8_t desc_length;
    enum usb_desc_type desc_type : 8;
    struct usb_endpoint_addr endpoint_addr;
    struct {
        usbtransfer_t transfer_type : 2;
        enum {
            NO_SYNC  = 0,   // no synchronisation
            ASYNC    = 1,   // asynchronous
            ADAPTIVE = 2,   // adaptive
            SYNC     = 3,   // synchronous
        } synchronisation : 2;
        enum {
            DATA = 0,
            FEEDBACK = 1,
            IMP_FEEDBACK_DATA = 2,  // implicit feedback data
        } usage : 2;
        uint8_t reserved : 2;   // bits 6-7
    } __attribute__((packed)) attribs;
    struct {
        unsigned int max_size : 11;
        enum {
            NONE = 0,
            EXTRA1 = 1,
            EXTRA2 = 2,
        } transactions : 2;
        uint8_t reserved : 3;   // bits 13-15
    } __attribute__((packed)) packet;
    uint8_t interval;
} __attribute__((packed));

/**
 * string descriptor info
 * 9.6.7
 */
struct usb_str_desc {
    uint8_t desc_length;
    enum usb_desc_type desc_type : 8;
    uint16_t data[];
} __attribute__((packed));

#endif
