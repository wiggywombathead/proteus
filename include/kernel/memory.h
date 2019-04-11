/**
 * @file memory.h
 * @brief Code to organise memory into pages and set aside a portion of memory
 * for the heap
 */

#ifndef _MEMORY_H
#define _MEMORY_H

#define PAGE_SIZE 4096
#define KERNEL_HEAP_SIZE (1024*1024)

#define KILOBYTE 1024
#define MEGABYTE (1024*1024)

#include <stdint.h>
#include <kernel/atag.h>
#include <kernel/list.h>

struct page_flags {
    uint8_t allocated : 1;
    uint8_t kernel_page : 1;
    uint8_t kernel_heap_page : 1;
    uint8_t shared : 1;
    uint32_t reserved : 28;
};

DEFINE_LIST(page);

struct page {
    uint32_t virt_addr;     // the virtual address mapping to this page
    struct page_flags flags;
    DEFINE_LINK(page);
};

struct heap_segment {
    struct heap_segment *prev;
    struct heap_segment *next;
    uint32_t allocated : 1;
    uint32_t segment_size;
};

void memory_init(struct atag *);
void *alloc_page(void);
void *alloc_shared(void);
void free_page(void *);
void *kmalloc(uint32_t);
void kfree(void *);

#endif
