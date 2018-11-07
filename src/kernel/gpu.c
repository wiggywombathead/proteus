#include <stdbool.h>
#include <kernel/gpu.h>
#include <kernel/framebuffer.h>
#include <kernel/fonts.h>

#include <common/stdlib.h>

/* initialise framebuffer and write black pixels everywhere */
void gpu_init(void) {

    static const struct pixel BLACK = { 0x00, 0x00, 0x00 };

    framebuffer_init();

    for (uint32_t j = 0; j < fb_info.height; j++) {
        for (uint32_t i = 0; i < fb_info.width; i++) {
            write_pixel(i, j, &BLACK);
        }
    }

}

void write_pixel(uint32_t x, uint32_t y, const struct pixel *p) {
    uint8_t *loc = fb_info.buffer + y * fb_info.pitch + x * BYTES_PER_PIXEL;
    memcpy(loc, (void *) p, BYTES_PER_PIXEL);
}

void gpu_putc(char c) {
    static const struct pixel ORANGE = { 0xff, 0x66, 0x00 };
    static const struct pixel BLACK = { 0x00, 0x00, 0x00 };

    bool printable = true;

    const uint8_t *bmp = (uint8_t *) dhepper8x8[(int) c];

    uint32_t i;

    /* scrolling */
    if (fb_info.row > fb_info.max_row) {

        // shift all characters up one row
        for (i = 0; i < fb_info.max_row - 1; i++) {
            memcpy(
                    fb_info.buffer + fb_info.pitch * i * CHAR_HEIGHT,
                    fb_info.buffer + fb_info.pitch * (i+1) * CHAR_HEIGHT,
                    fb_info.pitch * CHAR_HEIGHT
            );
        }

        // zero out the last row
        bzero(fb_info.buffer + fb_info.pitch * i * CHAR_HEIGHT, fb_info.pitch * CHAR_HEIGHT);

        fb_info.row--;
    } 

    /* non-printable characters */
    switch (c) {
    case '\b':
    case '\n':
    case '\r':
        printable = false;
        break;
    default:
        printable = true;
    }

    // newlines
    if (c == '\n' || c == '\r') {
        fb_info.col = 0;
        fb_info.row++;
    }

    // backspace
    if (c == '\b') {
        fb_info.col = (fb_info.col == 0) ? 0 : fb_info.col - 1;
    }

    uint8_t w, h;
    uint8_t mask;

    if (printable) {

        /* print character pixel by pixel */
        for (w = 0; w < CHAR_WIDTH; w++) {
            for (h = 0; h < CHAR_HEIGHT; h++) {
                mask = 1 << (w);

                if (bmp[h] & mask) {
                    write_pixel(
                            fb_info.col * CHAR_WIDTH + w,
                            fb_info.row * CHAR_HEIGHT + h,
                            &ORANGE
                            );
                } else {
                    write_pixel(
                            fb_info.col * CHAR_WIDTH + w,
                            fb_info.row * CHAR_HEIGHT + h,
                            &BLACK
                            );
                }
            }
        }

        fb_info.col++;
    }

    if (fb_info.col > fb_info.max_col - 1) {
        fb_info.col = 0;
        fb_info.row++;
    }

}
