#include <kernel/framebuffer.h>
#include <kernel/mailbox.h>
#include <kernel/gpu.h>

struct fb_init {
    uint32_t width;
    uint32_t height;
    uint32_t v_width;
    uint32_t v_height;
    uint32_t bytes;
    uint32_t depth;
    uint32_t ignore_x;
    uint32_t ignore_y;
    void    *pointer;
    uint32_t size;
};

struct fb_init fbinit __attribute__((aligned(16)));

int framebuffer_init(void) {

    struct mail_message msg;

    fbinit.width = 640;
    fbinit.height = 480;
    fbinit.v_width = fbinit.width;
    fbinit.v_height = fbinit.height;
    fbinit.depth = COLOR_DEPTH;

    msg.data = ((uint32_t) &fbinit + 0x40000000) >> 4;

    mailbox_send(msg, FRAMEBUF_CHANNEL);
    msg = mailbox_read(FRAMEBUF_CHANNEL);

    if (!msg.data)
        return -1;

    fb_info.width = fbinit.width;
    fb_info.height = fbinit.height;
    fb_info.pitch = fbinit.bytes;
    fb_info.buffer = fbinit.pointer;
    fb_info.bufsize = fbinit.size;
    fb_info.max_col = fb_info.width / CHAR_WIDTH;
    fb_info.max_row = fb_info.height / CHAR_HEIGHT;
    fb_info.col = fb_info.row = 0;

    return 0;
}
