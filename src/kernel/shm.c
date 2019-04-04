#include <kernel/shm.h>
#include <kernel/memory.h>
#include <common/stdlib.h>

struct shm_section *shm_open(const char *name) {

    struct shm_section *shared;

    shared = kmalloc(sizeof(struct shm_section));
    shared->addr = alloc_page();
    strcpy(shared->name, name);

    return shared;
}

void shm_write(struct shm_section *s, void *data, size_t bytes) {
    memcpy(s->data, data, bytes);
}

void *shm_read(struct shm_section *s, const char *name) {
    return s->data;
}
