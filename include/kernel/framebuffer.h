#ifndef _FRAMEBUFFER_H
#define _FRAMEBUFFER_H

#define COLOR_DEPTH 24
#define BYTES_PER_PIXEL 3
#define CHAR_WIDTH 8
#define CHAR_HEIGHT 8

#include <stdint.h>

struct framebuffer_info {
    uint32_t width;
    uint32_t height;
    uint32_t pitch;
    void    *buffer;
    uint32_t bufsize;
    uint32_t char_width;
    uint32_t char_height;
    uint32_t row;       // current row
    uint32_t col;       // current column
};

struct framebuffer_info fb_info;
int framebuffer_init(void);

#endif
