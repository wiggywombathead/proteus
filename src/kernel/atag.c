#include <kernel/atag.h>

uint32_t get_total_mem(struct atag *a) {
    
    while (a->tag != ATAG_NONE) {
        if (a->tag == ATAG_MEM) {
            return a->mem.size;
        }
        a = a + a->size;    /* move to next tag */
    }

    /* TODO: just return 0 in real thing */
    return 1024 * 1024 * 256;
}
