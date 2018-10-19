#ifndef _MEMORY_H
#define _MEMORY_H

#define PAGE_SIZE 4096
#define KERNEL_HEAP_SIZE (1024*1024)

#include <stdint.h>
#include <kernel/atag.h>
#include <kernel/list.h>

struct page_flags {
    uint8_t allocated : 1;
    uint8_t kernel_page : 1;
    uint8_t kernel_heap_page : 1;
    uint32_t reserved : 29;
};

struct page {
    uint32_t virt_addr;     // the virtual address mapping to this page
    struct page_flags flags;
    DEFINE_LINK(page);
};

struct heap_segment {
    struct heap_segment *prev;
    struct heap_segment *next;
    uint8_t allocated : 1;
    uint32_t segment_size;
};

void mem_init(struct atag *);
void *alloc_page(void);
void free_page(void *);
void *kmalloc(uint32_t);
void kfree(void *);

#endif
