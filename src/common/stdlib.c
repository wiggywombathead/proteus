#ifndef _STDLIB_H
#define _STDLIB_H

#include <kernel/uart.h>
#include <kernel/gpu.h>

void bzero(void *ptr, int bytes) {
    char *p = ptr;
    while (bytes--)
        *p++ = 0;
}

void *memcpy(void *dst, const void *src, size_t bytes) {
    char *d = dst;
    const char *s = src;
    while (bytes--)
        *d++ = *s++;
    return d;
}

char getc() {
    return uart_getc();
}

void putc(char c) {
    gpu_putc(c);
}

void puts(const char *str) {
    for (size_t i = 0; str[i] != '\0'; i++)
        putc(str[i]);
}

#endif
