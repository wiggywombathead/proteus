#ifndef _HID_H
#define _HID_H

#include <kernel/usb/types.h>
#include <kernel/usb/device.h>

/**
 * Human Interface Device descriptor
 * defined in USB HIB 1.11 manual, 6.2.1
 */
struct hid_desc {
    uint8_t desc_length;
    enum desc_type type : 8;
    uint16_t version;
    enum hid_country {
        UNSUPPORTED    = 0,
        ARABIC         = 1,
		BELGIAN        = 2,
		CANADIAN_BI    = 3,
		CANADIAN_FR    = 4,
		CZECH          = 5,
		DANISH         = 6,
		FINNISH        = 7,
		FRENCH         = 8,
		GERMAN         = 9,
		GREEK          = 10,
		HEBREW         = 11,
		HUNGARY        = 12,
		INTL           = 13,
		ITALIAN        = 14,
		JAPAN          = 15,
		KOREAN         = 16,
		LATIN_AMERICAN = 17,
		DUTCH          = 18,
		NORWEGIAN      = 19,
		PERSIAN        = 20,
		POLAND         = 21,
		PORTUGUESE     = 22,
		RUSSIAN        = 23,
		SLOVAKIAN      = 24,
		SPANISH        = 25,
		SWEDISH        = 26,
		SWISS_FR       = 27,
		SWISS_GE       = 28,
		SWITZERLAND    = 29,
		TAIWAN         = 30,
		TURKISH_Q      = 31,
		ENG_UK         = 32,
		ENG_US         = 33,
		YUGOSLAVIAN    = 34,
		TURKISH_F      = 35,
	} country : 8;
    uint8_t desc_count;
    struct {
        enum desc_type type : 8;
        uint16_t length;
    } __attribute__((__packed__)) optional_descriptors[];
} __attribute__((__packed__));

/**
 * types of HID reports
 * defined in USB HID 1.11 7.2.1
 */
enum hid_report_type {
    HID_INPUT = 1,
    HID_OUTPUT = 2,
    HID_FEATURE = 3,
};

/* device_driver field in usb_driver_data_header for HID devices */
#define DEV_DRIVER_HID 0x48494430

/**
 * HID specific data
 */
struct hid_device {
    struct usb_driver_data_header header;
    struct hid_desc *descriptor;
    struct hid_parser_result *parser_result;
    struct usb_driver_data_header *driver_data;

    /* HID event handlers */
    void (*hid_detach)(struct usb_device *device);
    void (*hid_free)(struct usb_device *device);
};

#define HID_USAGE_ATTACH_COUNT 10

/* attach interface of particular usage */
extern devcall (*hid_usage_attach[HID_USAGE_ATTACH_COUNT])(struct usb_device *dev, uint32_t interface_no);

/* retrieve HID report (USB HID 1.11, 7.2.1) */
devcall hid_get_report(struct usb_device *dev, enum hid_report_type type, uint8_t id, uint8_t interface, uint32_t buflen, void *buf);

/* send HID report request (USB HID 1.11, 7.2.2) */
devcall hid_set_report(struct usb_device *dev, enum hid_report_type type, uint8_t id, uint8_t interface, uint32_t buflen, void *buf);

/* update device with values of report */
devcall hid_write_device(struct usb_device *dev, uint8_t report);

/* update report with values from device */
devcall hid_read_device(struct usb_device *dev, uint8_t report);

/* enumerate HID device */
devcall hid_attach(struct usb_device *dev, uint32_t interface_no);

#endif
