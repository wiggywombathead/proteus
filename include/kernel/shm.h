#ifndef _SHM_H
#define _SHM_H

#include <stddef.h>

struct shm_section {
    void *addr;
    char name[32];
    char *data;
};

struct shm_section *shm_open(const char *name);
void shm_write(struct shm_section *s, void *data, size_t bytes);
void *shm_read(struct shm_section *s, const char *name);

#endif
