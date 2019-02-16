#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include <stdbool.h>

#define KBD_ENC_INPUT_BUF   0x60    /* read input buffer */
#define KBD_ENC_CMD_REG     0x64    /* command register */
#define KBD_CTRL_STATUS_REG 0x60    /* status register */
#define KBD_CTRL_CMD_REG    0x64    /* command register */

#define DATA_PORT 0x60
#define CMD_REG 0x64
#define STATUS_REG 0x64

bool kbd_disabled;

enum kbd_ctrl_status_mask {
    KBD_CTRL_MASK_OUTPUT_BUF      = 0x01,
    KBD_CTRL_MASK_INPUT_BUF       = 0x02,
    KBD_CTRL_MASK_SYSTEM          = 0x04,
    KBD_CTRL_MASK_CMD_DATA        = 0x08,
    KBD_CTRL_MASK_LOCKED          = 0x10,
    KBD_CTRL_MASK_AUX_OUTPUT_BUF  = 0x20,
    KBD_CTRL_MASK_TIMEOUT         = 0x40,
    KBD_CTRL_MASK_PARITY          = 0x80
};

/* keyboard encoder commands */
enum kbd_enc_cmd {
    KBD_ENC_SET_LED                 = 0xed,
    KBD_ENC_ECHO                    = 0xee,
    KBD_ENC_SET_ALT_SCODE           = 0xf0,
    KBD_ENC_IDENTIFY                = 0xf2,
    KBD_ENC_SET_RATE                = 0xf3,
    KBD_ENC_ENABLE_SCAN             = 0xf4,
    KBD_ENC_DISABLE_SCAN            = 0xf5,
    KBD_ENC_SET_DEFAULT             = 0xf6,

    /* set all keys */
    KBD_ENC_ALL_AUTO                = 0xf7,
    KBD_ENC_ALL_MAKE_RELEASE        = 0xf8,
    KBD_ENC_ALL_MAKE                = 0xf9,
    KBD_ENC_ALL_AUTO_MAKE_RELEASE   = 0xfa,

    /* set specific key */
    KBD_ENC_SET_AUTO                = 0xfb,
    KBD_ENC_SET_MAKE_RELEASE        = 0xfc,
    KBD_ENC_SET_MAKE                = 0xfd,
    KBD_ENC_RESEND                  = 0xfe,
    KBD_ENC_RESET                   = 0xff,
};

/* keyboard controller commands */
enum kbd_ctrl_cmd {
    KBD_CTRL_READ           = 0x20, /* read command byte */
    KBD_CTRL_WRITE          = 0x60, /* write command byte */
    KBD_CTRL_SELF_TEST      = 0xaa, /* self test */
    KBD_CTRL_INT_TEST       = 0xab, /* interface test */
    KBD_CTRL_KBD_DISABLE    = 0xad,
    KBD_CTRL_KBD_ENABLE     = 0xae,
    KBD_CTRL_READ_IN        = 0xc0, /* read input port */
    KBD_CTRL_READ_OUT       = 0xd0, /* read output port */
    KBD_CTRL_WRITE_OUT      = 0xd1, /* write output port */
    KBD_CTRL_READ_TEST      = 0xe0,
    KBD_CTRL_SYS_RESET      = 0xfe,
    KBD_CTRL_MOUSE_DISABLE  = 0xa7,
    KBD_CTRL_MOUSE_ENABLE   = 0xa8,
    KBD_CTRL_MOUSE_TEST     = 0xa9, /* test mouse port */
    KBD_CTRL_MOUSE_WRITE    = 0xd4, /* write to mouse */
};

/* bytes keyboard may send as response to command */
enum kbd_response {
    KBD_BUF_OVERRUN         = 0x00,
    KBD_SELF_TEST_PASSED    = 0xaa,
    KBD_ECHO_RESPONSE       = 0xee,
    KBD_CMD_ACK             = 0xfa,
    KBD_SELF_TEST_FAILED    = 0xfc,
    KBD_RESEND              = 0xfe,
    KBD_KEY_ERR             = 0xff,
};

uint8_t kbd_ctrl_read_status(void); /* read kbd control status register */ 
void kbd_ctrl_send_cmd(uint8_t);    /* send command to kbd control */
uint8_t kbd_enc_read_buf(void);     /* read kbd input buffer */
void kbd_enc_send_cmd(uint8_t);     /* send command to kbd controller */

void kbd_set_leds(bool, bool, bool);    /* set LEDs on keyboard */
void kbd_set_led(uint8_t);

bool kbd_self_test(void);
void kbd_disable(void);
void kbd_reset_system(void);

#endif
