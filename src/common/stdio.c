#include <stddef.h>
#include <stdarg.h>
#include <kernel/gpio.h>
#include <kernel/gpu.h>
#include <common/stdio.h>
#include <common/stdlib.h>
#include <common/string.h>

char getc() {
	return uart_getc();
}

void putc(char c) {
	gpu_putc(c);
}

static void _puts(const char *str) {
	for (size_t i = 0; str[i] != '\0'; i++)
		putc(str[i]);
}

void puts(const char *str) {
	_puts(str);
	putc('\n');
}

void hexstring(uint32_t n) {
	char *symbols = "012345678abcdef";
	char buf[8];
	uint32_t i = 0;

	while (n > 0) {
		buf[i++] = symbols[n % 16];
		n /= 16;
	}

	_puts("0x");
	uint32_t len = i;
	for (i = len; i < 8; i++)
		putc('0');

	for (i = len - 1; ; i--) {
		putc(buf[i]);
		if (i == 0) break;
	}

	putc('\n');
}

int printf(const char *fmt, ...) {

	va_list args;
	va_start(args, fmt);

	int written = 0;

	int int_tmp;
	char char_tmp;
	char *str_tmp;

	/* TODO
	 *  float float_tmp;
	 *  void *ptr;
	 */

	while (*fmt != '\0') {

		if (*fmt == '%') {

			fmt++;
			char buf[64];

			switch (*fmt++) {
				case '%':
					putc('%');
					written++;
					break;
				case 'c':
					char_tmp = (char) va_arg(args, int);
					putc(char_tmp);
					written++;
					break;
				case 'd':
					int_tmp = va_arg(args, int);
					if (int_tmp < 0) {
						putc('-');
						int_tmp = -int_tmp;
					}
					itoa(int_tmp, 10, buf, 64);
					_puts(buf);
					written += strlen(buf);
					break;
				case 'o':
					int_tmp = va_arg(args, int);
					itoa(int_tmp, 8, buf, 64);
					_puts(buf);
					written += strlen(buf);
					break;
				case 'p':
					int_tmp = va_arg(args, int);
					itoa(int_tmp, 16, buf, 64);
					_puts("0x");
					_puts(buf);
					written += strlen(buf);
					break;
				case 's':
					str_tmp = va_arg(args, char *);
					_puts(str_tmp);
					written += strlen(str_tmp);
					break;
				case 'x':
					int_tmp = va_arg(args, int);
					itoa(int_tmp, 16, buf, 64);
					_puts(buf);
					written += strlen(buf);
					break;
			}

		} else {
			putc(*fmt++);
			written++;
		}
	}

	va_end(args);
	return written;

}
