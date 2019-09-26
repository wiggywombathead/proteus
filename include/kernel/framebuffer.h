/**
 * @file framebuffer.h
 * @brief Code to interact with the GPU via framebuffer
 */

#ifndef _FRAMEBUFFER_H
#define _FRAMEBUFFER_H

#include <stdint.h>

#define COLOR_DEPTH 24
#define BYTES_PER_PIXEL 3

struct framebuffer {
	uint32_t width;
	uint32_t height;
	uint32_t pitch;
	void    *buffer;
	uint32_t bufsize;
	uint32_t max_col;
	uint32_t max_row;
	uint32_t col;       // current column
	uint32_t row;       // current row
};

struct framebuffer fb_info;
int framebuffer_init(void);

#endif
