#include <kernel/atag.h>

uint32_t get_total_mem(struct atag *atag) {
    
    while (atag->tag != ATAG_NONE) {
        if (atag->tag == ATAG_MEM) {
            return atag->atag_mem.size;
        }
        a = a + a->size;    /* move to next tag */
    }

    /* TODO: just return 0 in real thing */
    return 1024 * 1024 * 256;
}
