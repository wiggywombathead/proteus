/**
 * @file stdio.h
 * @brief Standard I/O functions
 */

#ifndef _STDIO_H
#define _STDIO_H

char getc(void);
void putc(char);
void puts(const char *);
void hexstring(uint32_t);
int printf(const char *, ...);

#endif
