#include <kernel/shm.h>
#include <kernel/gpio.h>
#include <kernel/memory.h>

#include <common/stdio.h>
#include <common/string.h>

extern struct page *all_pages;

void *shm_open() {

    struct shm_section *shared;
    shared = kmalloc(sizeof(struct shm_section));
    shared->addr = alloc_shared();

    return shared->addr;
}

void shm_write(void *addr, void *data, size_t bytes) {
    // memcpy(addr, data, bytes);
    (void) bytes;
    mmio_write(addr, &data);
}

void *shm_read(void *addr) {
    struct page *page;
    page = all_pages + ((uint32_t) addr / PAGE_SIZE);

    if (!page->flags.allocated) {
        puts("Error: page not allocated");
        return NULL;
    }

    if (!page->flags.shared) {
        puts("Illegal access: page not open for sharing");
        return NULL;
    }

    return (void *) mmio_read((uint32_t) addr);
}
