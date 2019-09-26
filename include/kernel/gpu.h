/**
 * @file gpu.h
 * @brief Code to deal with writing pixels to the screen
 */

#ifndef _GPU_H
#define _GPU_H

#include <stdint.h>

#define CHAR_WIDTH 8
#define CHAR_HEIGHT 8

struct pixel {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
};

int gpu_init(void);
void write_pixel(uint32_t, uint32_t, const struct pixel *);
void gpu_putc(char);
void clrs(void);

uint32_t get_screen_width(void);
uint32_t get_screen_height(void);
uint32_t get_console_width(void);
uint32_t get_console_height(void);

#endif
