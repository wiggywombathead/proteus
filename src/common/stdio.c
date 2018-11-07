#include <stddef.h>
#include <stdarg.h>
#include <kernel/uart.h>
#include <kernel/gpu.h>
#include <common/stdio.h>
#include <common/stdlib.h>

int printf(const char *fmt, ...) {

    va_list args;
    va_start(args, fmt);

    int written = 0;

    int int_tmp;
    float float_tmp;
    char char_tmp;
    char *str_tmp;

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
                    char_tmp = va_arg(args, int);
                    break;
                case 'd':
                    int_tmp = va_arg(args, int);
                    // TODO: itoa(int_tmp, buf, ...)
                    puts(buf);
                    written += strlen(buf);
                    break;
                case 'p':
                    break;
                case 's':
                    break;
                case 'x':
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
