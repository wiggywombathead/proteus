#include <common/string.h>
#include <stdint.h>

void bzero(void *ptr, int bytes) {
    memset(ptr, 0, bytes);
}

void *memset(void *ptr, int c, size_t bytes) {
    uint8_t *p = ptr;
    while (bytes--)
        *p++ = c;
    return ptr;
}

void *memcpy(void *dst, const void *src, size_t bytes) {
    char *d = dst;
    const char *s = src;
    while (bytes--)
        *d++ = *s++;
    return d;
}

int memcmp(const void *s1, const void *s2, size_t n) {
    const unsigned char *buf1 = (const unsigned char *) s1;
    const unsigned char *buf2 = (const unsigned char *) s2;

    while (n-- > 0) {
        if (*buf1 != *buf2)
            return (*(unsigned char *) buf1 - *(unsigned char *) buf2);
        buf1++;
        buf2++;
    }
    return 0;
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

size_t strlen(const char *str) {
    size_t len = 0;
    while (*str++ != '\0')
        len++;
    return len;
}

int strcmp(const char *s1, const char *s2) {
    while (1) {
        if (*s1 != *s2)
            return *s1 - *s2;
        if (*s1 == '\0')
            return 0;

        s1++;
        s2++;
    }
}

char *strcat(char *dst, const char *src) {
    size_t len = strlen(dst);
    size_t i;
    for (i = 0; src[i] != '\0'; i++) {
        dst[len + i] = src[i];
    }
    dst[len + i] = '\0';
    return dst;
}
