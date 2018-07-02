#include <kernel/memory.h>
#include <kernel/atag.h>
#include <common/stdlib.h>

extern uint8_t __end;
static uint32_t pages;

DEFINE_LIST(page);
IMPLEMENT_LIST(page);

static struct page *all_pages;
struct page_list free_pages;

static struct heap_segment *heap_segment_list_head;

static void heap_init(uint32_t heap_start) {
    heap_segment_list_head = (struct heap_segment *) heap_start;
    bzero(heap_segment_list_head, sizeof(struct heap_segment));
    heap_segment_list_head->segment_size = KERNEL_HEAP_SIZE;
}

void mem_init(struct atag atags) {

    uint32_t total_mem, kernel_pages, page_array_end, page_array_len;

    total_mem = get_total_mem(&atags);
    pages = total_mem / PAGE_SIZE;

    page_array_len = sizeof(struct page) * pages;
    all_pages = (struct page *) &__end;

    bzero(all_pages, page_array_len);
    INIT_LIST(free_pages);

    kernel_pages = ((uint32_t) &__end) / PAGE_SIZE;

    uint32_t i;
    for (i = 0; i < kernel_pages; i++) {
        all_pages[i].virt_addr = i * PAGE_SIZE;
        all_pages[i].flags.allocated = 1;
        all_pages[i].flags.kernel_page = 1;
    }

    for ( ; i < kernel_pages + (KERNEL_HEAP_SIZE / PAGE_SIZE); i++) {
        all_pages[i].virt_addr = i * PAGE_SIZE;
        all_pages[i].flags.allocated = 1;
        all_pages[i].flags.kernel_heap_page = 1;
    }

    for ( ; i < pages; i++) {
        all_pages[i].flags.allocated = 0;
        append_page_list(&free_pages, &all_pages[i]);
    }

    page_array_end = (uint32_t) &__end + page_array_len;
    heap_init(page_array_end);

}

void *alloc_page(void) {

    struct page *page;
    void *page_mem;

    if (size_page_list(&free_pages) == 0)
        return 0;

    page = dequeue_page_list(&free_pages);
    page->flags.kernel_page = 1;
    page->flags.allocated = 1;

    page_mem = (void *) ((page - all_pages) * PAGE_SIZE);
    bzero(page_mem, PAGE_SIZE);

    return page_mem;
}

void free_page(void *ptr) {

    struct page *page;
    page = all_pages + ((uint32_t) ptr / PAGE_SIZE);

    page->flags.allocated = 0;
    append_page_list(&free_pages, page);

}

void *kmalloc(uint32_t bytes) {

    struct heap_segment *curr, *best;
    int diff, best_diff;

    best = NULL;
    best_diff = 0x7fffffff;     /* max signed int */

    /* ensure we have enough to satisfy request and header */
    bytes += sizeof(struct heap_segment);

    /* make it 16-byte aligned */
    if (bytes % 16 != 0)
        bytes += 16 - (bytes % 16);

    for (curr = heap_segment_list_head; curr != NULL; curr = curr->next) {

        diff = curr->segment_size - bytes;

        if (!curr->allocated && diff < best_diff && diff >= 0) {
            best = curr;
            best_diff = diff;
        }

    }

    /* if no free memory, best is never assigned */
    if (best == NULL)
        return NULL;

    best->allocated = 1;
    return best + 1;        /* return address of next segment */
}

void kfree(void *ptr) {

    struct heap_segment *seg = ptr - sizeof(struct heap_segment);
    seg->allocated = 0;

    while (seg->prev != NULL && !seg->next->allocated) {
        seg->segment_size += seg->next->segment_size;
        seg->next = seg->next->next;
    }

}
