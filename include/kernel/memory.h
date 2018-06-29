#ifndef _MEMORY_H
#define _MEMORY_H

#define PAGE_SIZE 4096

#include <stdint.h>
#include <kernel/atag.h>
#include <kernel/list.h>

struct page_flags {
    uint8_t allocated : 1;
    uint8_t kernel_page : 1;
    uint32_t reserved : 30;
};

struct page {
    uint32_t virt_addr;
    struct page_flags flags;
    DEFINE_LINK(page);
};

void mem_init(struct atag);
void *alloc_page(void);
void free_page(void *);

#endif
