#include <stddef.h>
#include <stdint.h>
#include <common/stdlib.h>

void memset(void *ptr, uint8_t c, size_t bytes) {
    uint8_t *p = ptr;
    while (bytes--)
        *p++ = c;
}

void bzero(void *ptr, int bytes) {
    memset(ptr, 0, bytes);
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

size_t strcpy(char *dst, const char *src) {

    size_t i;
    for (i = 0; src[i] != '\0'; i++)
        dst[i] = src[i];

    dst[i] = '\0';
    return i;
}

size_t strncpy(char *dst, const char *src, size_t n) {

    size_t i;
    for (i = 0; i < n && src[i] != '\0'; i++)
        dst[i] = src[i];

    size_t len = i;

    for ( ; i < n; i++)
        dst[i] = '\0';

    return len;
}

uint32_t itoa(uint32_t n, uint32_t base, char *buf, size_t max) {
    char *symbols = "0123456789abcdef";
    uint32_t i = 0;

    if (n == 0) {
        return strncpy(buf, "0", 2);
    }
        
    while (n > 0) {
        if (i > max - 1)
            return -1;
        buf[i++] = symbols[n % base];
        n /= base;
    }

    uint32_t len = i;
    buf[len] = '\0';

    for (i = 0; i < len / 2; i++) {
        char tmp = buf[i];
        buf[i] = buf[len-1-i];
        buf[len-1-i] = tmp;
    }

    return len;
}

size_t strlen(const char *str) {
    size_t len = 0;
    while (*str++ != '\0')
        len++;
    return len;
}
