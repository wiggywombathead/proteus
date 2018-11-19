#ifndef _GPU_H
#define _GPU_H

#include <stdint.h>

struct pixel {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

int gpu_init(void);
void write_pixel(uint32_t, uint32_t, const struct pixel *);
void gpu_putc(char);

#endif
