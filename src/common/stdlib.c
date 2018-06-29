#ifndef _STDLIB_H
#define _STDLIB_H

void bzero(void *ptr, int bytes) {

    char *p = ptr;
    while (bytes--)
        *p++ = 0;

}

#endif
