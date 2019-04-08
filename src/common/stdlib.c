#include <common/stdlib.h>
#include <common/string.h>

void *memset(void *ptr, int c, size_t bytes) {
    uint8_t *p = ptr;
    while (bytes--)
        *p++ = c;
    return ptr;
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

uint32_t atoi(const char *str) {
    uint32_t ans = 0;
    while (*str != '\0') {
        ans = ans * 10 + (*str - '0');
        str++;
    }
    return ans;
}

uint32_t itoa(uint32_t n, uint32_t base, char *buf, size_t max) {
    char *symbols = "0123456789abcdef";
    uint32_t i = 0;

    if (n == 0) {
        memcpy(buf, "0", 2);
        return 1;
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
