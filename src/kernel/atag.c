#include <kernel/atag.h>

#include <common/stdlib.h>
#include <kernel/gpio.h>

uint32_t get_total_mem(struct atag *atag) {
    
    int blinks = 0;

    while (atag->tag != ATAG_NONE) {
        if (atag->tag == ATAG_MEM)
            return atag->mem.size;

        switch (atag->tag) {
        case ATAG_CORE:
            uart_puts("core\n");
            act_blink(3);
            break;
        case ATAG_MEM:
            uart_puts("mem\n");
            act_blink(4);
            break;
        case ATAG_VIDEOTEXT: blinks = 3;
            break;
        case ATAG_RAMDISK: blinks = 4;
            break;
        case ATAG_INITRD2: blinks = 5;
            break;
        case ATAG_SERIAL: blinks = 6;
            break;
        case ATAG_REVISION: blinks = 7;
            break;
        case ATAG_VIDEOLFB: blinks = 8;
            break;
        case ATAG_CMDLINE: blinks = 9;
            break;
        default: blinks = 10;
        }
        
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
