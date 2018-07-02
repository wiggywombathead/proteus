#ifndef _MAILBOX_H
#define _MAILBOX_H

#include <stddef.h>

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

struct fb_allocate_res {
    void *fb_addr;
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
    uint32_t fb_bits_per_pixel;
    uint32_t fb_bytes_per_row;
};

struct property_msg_tag {
    struct property_tag prop_tag;
    struct value_buffer value_buffer;
};

enum buf_req_res_code {
    REQUEST = 0x00000000,
    RESPONSE_SUCCESS = 0x80000000,
    RESPONSE_ERROR = 0x80000001
};

struct property_msg_buf {
    uint32_t size;
    struct buf_req_res_code req_res_code;
    uint32_t tags[1];
};

#endif
