#ifndef _MAILBOX_H
#define _MAILBOX_H

#include <stddef.h>
#include <stdint.h>

#include <kernel/peripheral.h>

#define MAILBOX_BASE    PERIPHERAL_BASE + MAILBOX_OFFSET
#define MAIL0_READ      (((struct mail_message *) (MAILBOX_BASE + 0x00)))
#define MAIL0_STATUS    (((struct mail_status *) (MAILBOX_BASE + 0x18)))
#define MAIL0_WRITE     (((struct mail_message *) (MAILBOX_BASE + 0x20)))

#define FRAMEBUF_CHANNEL 1
#define PROPERTY_CHANNEL 8

struct mail_message {
    uint8_t channel : 4;
    uint32_t data : 28;
};

struct mail_status {
    uint32_t reserved : 30;
    uint8_t empty : 1;
    uint8_t full : 1;
};

enum property_tag {
    NULL_TAG                    = 0,
    FB_ALLOCATE_BUFFER          = 0x00040001,
    FB_RELEASE_BUFFER           = 0x00048001,
    FB_GET_PHYSICAL_DIMENSIONS  = 0x00040003,
    FB_SET_PHYSICAL_DIMENSIONS  = 0x00048003,
    FB_GET_VIRTUAL_DIMENSIONS   = 0x00040004,
    FB_SET_VIRTUAL_DIMENSIONS   = 0x00048004,
    FB_GET_BITS_PER_PIXEL       = 0x00040005,
    FB_SET_BITS_PER_PIXEL       = 0x00048005,
    FB_GET_BYTES_PER_ROW        = 0x00040008
};

enum buf_req_res_code {
    REQUEST             = 0x00000000,
    RESPONSE_SUCCESS    = 0x80000000,
    RESPONSE_ERROR      = 0x80000001
};

struct fb_allocate_res {
    void    *fb_addr;
    uint32_t fb_size;
};

struct fb_screen_size {
    uint32_t width;
    uint32_t height;
};

union value_buffer {
    uint32_t fb_allocate_align;
    struct fb_allocate_res fb_allocate_res;
    struct fb_screen_size fb_screen_size;
    uint32_t fb_depth;  // bits per pixel
    uint32_t fb_pitch;  // bytes per row
};

struct property_msg_tag {
    enum property_tag prop_tag;
    union value_buffer value_buffer;
};

struct property_msg_buf {
    uint32_t size;
    enum buf_req_res_code req_res_code;
    uint32_t tags[1];
};

struct mail_message mailbox_read(int);
void mailbox_send(struct mail_message, int);
int send_messages(struct property_msg_tag *);

#endif
