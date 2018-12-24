#include <kernel/atag.h>

uint32_t get_total_mem(struct atag *atag) {
    
    int blinks = 0;

    while (atag->tag != ATAG_NONE) {
        if (atag->tag == ATAG_MEM)
            return atag->mem.size;

        /* move to next tag */
        atag = (struct atag *) (((uint32_t *) atag) + atag->size);
    }

    return 0;
}
uint32_t get_serialnr_hi(struct atag *atag) {
    
    while (atag->tag != ATAG_NONE) {
        if (atag->tag == ATAG_SERIAL)
            return atag->serialnr.high;

        /* move to next tag */
        atag = (struct atag *)(((uint32_t *) atag) + atag->size);
    }

    return 0;
}

uint32_t get_serialnr_lo(struct atag *atag) {
    
    while (atag->tag != ATAG_NONE) {
        if (atag->tag == ATAG_SERIAL)
            return atag->serialnr.low;

        /* move to next tag */
        atag = (struct atag *)(((uint32_t *) atag) + atag->size);
    }

    return 0;
}
