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

    const uint8_t *bmp = (uint8_t *) dhepper8x8[(int) c];

    uint32_t i;
    uint32_t num_rows = fb_info.height / CHAR_HEIGHT;

    if (fb_info.row >= num_rows) {

        // shift all characters up one row
        for (i = 0; i < num_rows - 1; i++) {
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

    if (c == '\n') {
        fb_info.col = 0;
        fb_info.row++;
        return;
    }

    uint8_t w, h;
    uint8_t mask;

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

    if (fb_info.col > fb_info.char_width) {
        fb_info.col = 0;
        fb_info.row++;
    }

}
