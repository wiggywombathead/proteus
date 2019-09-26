#include <common/stdlib.h>
#include <kernel/memory.h>
#include <common/string.h>

void *malloc(size_t size) {
	return kmalloc(size);
}

void free(void *ptr) {
	kfree(ptr);
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
