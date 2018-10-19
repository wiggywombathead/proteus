#include <kernel/mailbox.h>

int framebuffer_init(void) {

    struct property_msg_tag tags[5];

    tags[0].proptag = FB_SET_PHYSICAL_DIMENSIONS;
    tags[0].value_buffer.fb_screen_size.width = 640;
    tags[0].value_buffer.fb_screen_size.height = 480;

    tags[1].proptag = FB_SET_VIRTUAL_DIMENSIONS;
    tags[1].value_buffer.fb_screen_size.width = 640;
    tags[1].value_buffer.fb_screen_size.height = 480;

    tags[2].proptag = FB_SET_BITS_PER_PIXEL;
    tags[2].value_buffer.fb_depth = COLOR_DEPTH;

    tags[3].proptag = NULL;

    if (send_messages(tags) != 0) {
        return -1;
    }

    // TODO: finish here

