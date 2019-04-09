/**
 * @file stdlib.h
 * @brief Implementation of functions from C stdlib.h
 */

#ifndef _STDLIB_H
#define _STDLIB_H

#include <stddef.h>
#include <stdint.h>

void *malloc(size_t);
void free(void *);

uint32_t atoi(const char *);
uint32_t itoa(uint32_t, uint32_t, char *, size_t);

#endif
