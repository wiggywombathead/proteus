#ifndef _STDLIB_H
#define _STDLIB_H

#include <stddef.h>

void bzero(void *, int);
void *memcpy(void *, void *, size_t);
char getc(void);
void putc(char);
void puts(const char *);

#endif
