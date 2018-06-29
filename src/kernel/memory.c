#include <kernel/memory.h>
#include <kernel/atag.h>
#include <common/stdlib.h>

extern uint8_t __end;
static uint32_t pages;

DEFINE_LIST(page);
IMPLEMENT_LIST(page);

static struct page *all_pages;
struct page_list free_pages;

void mem_init(struct atag atags) {

    uint32_t total_mem, kernel_pages, page_array_len;

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

    for ( ; i < pages; i++) {
        all_pages[i].flags.allocated = 0;
        append_page_list(&free_pages, &all_pages[i]);
    }

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
