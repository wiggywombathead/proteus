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

uint32_t atoi(const char *nstr) {
    uint32_t ans = 0;
    while (*nstr != '\0') {
        ans = ans * 10 + (*nstr - '0');
        nstr++;
    }
    return ans;
}

uint32_t strlen(const char *str) {
    uint32_t len;
    while (*str++ != '\0')
        len++;
    return len;
}

#endif
