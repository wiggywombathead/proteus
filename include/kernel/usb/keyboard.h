#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include <kernel/usb/types.h>
#include <kernel/usb/device.h>

/* current state of keyboard modifier keys */
struct kbd_modifiers {
    bool lctrl : 1;
    bool lshift : 1;
    bool lalt : 1;
    bool lgui : 1;
    bool rctrl : 1;
    bool rshift : 1;
    bool ralt : 1;
    bool rgui : 1;
} __attribute__((__packed__));

/* current state of keyboard LEDS */
struct kbd_leds {
    bool numlock : 1;
    bool capslock : 1;
    bool scrllock : 1;
    bool compose : 1;
    bool kana : 1;
    bool power : 1;
    bool mute : 1;
    bool shift : 1;
} __attribute__((__packed__));

/* device_driver field in usb_driver_data_header for keyboard */
#define DEV_DRIVER_KBD 0x4B424430

/* max number of keys reported simultaneously */
#define KBD_MAX_KEYS 6

/* keyboard specific data */
struct kbd_device {
    struct usb_driver_data_header header;
    uint32_t index;
    uint32_t keycount;
    uint16_t keys_down[KBD_MAX_KEYS];
    struct kbd_modifiers modifiers;
    struct kbd_leds leds_supported;

    struct hid_parser_field *led_fields[8];
    struct hid_parser_field *key_fields[8+1];

    struct hid_parser_report *led_report;
    struct hid_parser_report *key_report;
};

/* enumerate keyboard device */
devcall kbd_attach(struct usb_device *dev, uint32_t interface);

/* return number of keyboards attached */
uint32_t kbd_count(void);

/* set keyboard LEDS */
devcall kbd_set_leds(uint32_t kbd_addr, struct kbd_leds leds);

/* get a list of available keyboard LEDs */
struct kbd_leds kbd_get_led_support(uint32_t kbd_addr);

/* checks a given keyboard */
devcall kbd_poll(uint32_t kbd_addr);

/* read modifier keys */
struct kbd_modifiers kbd_get_mods(uint32_t kbd_addr);

/* return number of keys held down */
uint32_t kbd_get_keys_down(uint32_t kbd_addr);

/* return state of given key */
bool key_is_down(uint32_t kbd_addr, uint16_t key);

/* return nth key down */
uint16_t kbd_get_key_down(uint32_t kbd_addr, uint32_t index);

/* return the address of nth connected keyboard */
uint32_t kbd_get_addr(uint32_t index);

#endif
