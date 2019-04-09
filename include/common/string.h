/**
 * @file string.h
 * @brief String functions
 */

#ifndef _STRING_H
#define _STRING_H

#include <stddef.h>

void bzero(void *, int);

void *memset(void *, int, size_t);
void *memcpy(void *, const void *, size_t);
int memcmp(const void *, const void *, size_t);


size_t strcpy(char *, const char *);
size_t strncpy(char *, const char *, size_t);
size_t strlen(const char *);
int strcmp(const char *, const char *);
char *strcat(char *, const char *);

#endif
