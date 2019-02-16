#include <kernel/memory.h>
#include <kernel/atag.h>
#include <common/stdlib.h>

extern uint8_t __end;
static uint32_t pages;

IMPLEMENT_LIST(page);

static struct page *all_pages;
struct page_list free_pages;

static struct heap_segment *heap_segment_list_head;

static void heap_init(uint32_t heap_start) {
    heap_segment_list_head = (struct heap_segment *) heap_start;
    bzero(heap_segment_list_head, sizeof(struct heap_segment));
    heap_segment_list_head->segment_size = KERNEL_HEAP_SIZE;
}

void mem_init(struct atag *atags) {

    uint32_t total_mem, kernel_pages, page_array_end, page_array_len;

    total_mem = get_total_mem(atags);   // total memory of system
    pages = total_mem / PAGE_SIZE;      // total number of pages

    // create array of pages directly after end of kernel img
    all_pages = (struct page *) &__end;
    page_array_len = sizeof(struct page) * pages;

    bzero(all_pages, page_array_len);
    INIT_LIST(free_pages);

    // determine number of pages available
    kernel_pages = ((uint32_t) &__end) / PAGE_SIZE;

    // mark all pages with appropriate flags, starting with kernel pages
    uint32_t i;
    for (i = 0; i < kernel_pages; i++) {
        all_pages[i].virt_addr = i * PAGE_SIZE;
        all_pages[i].flags.allocated = 1;
        all_pages[i].flags.kernel_page = 1;
    }

    // reserve 1MB for kernel heap
    for ( ; i < kernel_pages + (KERNEL_HEAP_SIZE / PAGE_SIZE); i++) {
        all_pages[i].virt_addr = i * PAGE_SIZE;
        all_pages[i].flags.allocated = 1;
        all_pages[i].flags.kernel_heap_page = 1;
    }

    // do the same for regular pages
    for ( ; i < pages; i++) {
        all_pages[i].flags.allocated = 0;
        append_page_list(&free_pages, &all_pages[i]);
    }

    page_array_end = (uint32_t) &__end + page_array_len;
    heap_init(page_array_end);

}

void *alloc_page(void) {

    struct page *page;
    void *page_addr;

    if (size_page_list(&free_pages) == 0)
        return 0;

    // allocate the first page in the free_page list
    page = dequeue_page_list(&free_pages);
    page->flags.kernel_page = 1;
    page->flags.allocated = 1;

    // find the physical address of this page in memory
    page_addr = (void *) ((page - all_pages) * PAGE_SIZE);
    bzero(page_addr, PAGE_SIZE);

    return page_addr;
}

void free_page(void *ptr) {

    // find the page's index using its physical address
    struct page *page;
    page = all_pages + ((uint32_t) ptr / PAGE_SIZE);

    // deallocate page and add back to free_pages list
    page->flags.allocated = 0;
    append_page_list(&free_pages, page);

}

void *kmalloc(uint32_t bytes) {

    struct heap_segment *curr, *best;
    int diff, best_diff;

    best = NULL;
    best_diff = 0x7fffffff;     // max signed int

    // ensure we have enough to satisfy request and header
    bytes += sizeof(struct heap_segment);

    // make it 16-byte aligned
    if (bytes % 16 != 0)
        bytes += 16 - (bytes % 16);

    // find an allocation best matching the request
    for (curr = heap_segment_list_head; curr != NULL; curr = curr->next) {

        diff = curr->segment_size - bytes;

        if (!curr->allocated && diff < best_diff && diff >= 0) {
            best = curr;
            best_diff = diff;
        }

    }

    // no free memory to allocate
    if (best == NULL)
        return NULL;

    // split this segment if its size >= twice the header size
    if (best_diff > (int) (2 * sizeof(struct heap_segment))) {
        bzero(((void *) best) + bytes, sizeof(struct heap_segment));
        curr = best->next;
        best->next = ((void *) best) + bytes;
        best->next->next = curr;
        best->next->prev = best;
        best->next->segment_size = best->segment_size - bytes;
        best->segment_size = bytes;
    }

    best->allocated = 1;
    return best + 1;        // return address of next segment
}

void kfree(void *ptr) {

    struct heap_segment *seg = ptr - sizeof(struct heap_segment);
    seg->allocated = 0;

    /* merge segments to the left */
    while (seg->prev != NULL && !seg->prev->allocated) {
        seg->prev->next = seg->next;
        seg->prev->segment_size += seg->segment_size;
        seg = seg->prev;
    }

    /* merge segments to the right */
    while (seg->next != NULL && !seg->next->allocated) {
        seg->next->next->prev = seg;
        seg->next = seg->next->next;
        seg->segment_size += seg->next->segment_size;
    }

}
