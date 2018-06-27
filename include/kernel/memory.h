#ifndef _MEMORY_H
#define _MEMORY_H

#include <stdint.h>

struct page_flags {
    uint8_t allocated : 1;
    uint8_t kernel_page : 1;
    uint32_t reserved : 30;
};

struct page {
    uint32_t virt_addr;
    struct page_flags flags;
    // TODO: DEFINE_LINK(struct page);
};

#endif
