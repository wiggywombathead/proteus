#include <kernel/atag.h>

uint32_t get_total_mem(struct atag *atag) {
    
    while (atag->tag != ATAG_NONE) {
        if (atag->tag == ATAG_MEM) {
            return atag->mem.size;
        }
        
        /* move to next tag */
        atag = (struct atag *)(((uint32_t *) atag) + atag->size);
    }

    return 0;
}
