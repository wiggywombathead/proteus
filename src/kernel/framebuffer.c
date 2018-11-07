#include <kernel/framebuffer.h>
#include <kernel/mailbox.h>
#include <kernel/gpu.h>

/* set up framebuffer channel */
int framebuffer_init(void) {

    struct property_msg_tag tags[5];

    tags[0].prop_tag = FB_SET_PHYSICAL_DIMENSIONS;
    tags[0].value_buffer.fb_screen_size.width = 640;
    tags[0].value_buffer.fb_screen_size.height = 480;

    tags[1].prop_tag = FB_SET_VIRTUAL_DIMENSIONS;
    tags[1].value_buffer.fb_screen_size.width = 640;
    tags[1].value_buffer.fb_screen_size.height = 480;

    tags[2].prop_tag = FB_SET_BITS_PER_PIXEL;
    tags[2].value_buffer.fb_depth = COLOR_DEPTH;

    tags[3].prop_tag = NULL_TAG;

    if (send_messages(tags) != 0) {
        return -1;
    }

    fb_info.width = tags[0].value_buffer.fb_screen_size.width;
    fb_info.height = tags[0].value_buffer.fb_screen_size.height;

    fb_info.max_col = fb_info.width / CHAR_WIDTH;
    fb_info.max_row = fb_info.height / CHAR_HEIGHT;

    fb_info.col = 0;
    fb_info.row = 0;

    fb_info.pitch = fb_info.width * BYTES_PER_PIXEL;

    // request a framebuffer
    tags[0].prop_tag = FB_ALLOCATE_BUFFER;
    tags[0].value_buffer.fb_screen_size.width = 0;
    tags[0].value_buffer.fb_screen_size.height = 0;
    tags[0].value_buffer.fb_allocate_align = 16;
    tags[1].prop_tag = NULL_TAG;

    if (send_messages(tags) != 0)
        return -1;

    fb_info.buffer = tags[0].value_buffer.fb_allocate_res.fb_addr;
    fb_info.bufsize = tags[0].value_buffer.fb_allocate_res.fb_size;

    return 0;
}
