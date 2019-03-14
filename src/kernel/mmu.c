#include <kernel/mmu.h>

#define MMU_TABLE_BASE 0x00004000

#define BUFFERABLE  0x04
#define CACHEABLE   0x08

void mmu_entry(uint32_t virt, uint32_t phys, uint32_t flags) {

    uint32_t offset = virt >> 20;
    uint32_t *entry = (uint32_t *) (MMU_TABLE_BASE | (offset << 2));

    // mask lower 20 bits of physical address, then add flags and 0x02 (small)
    uint32_t physical = (phys & 0xfff00000) | (flags & 0x7ffa) | 0x02;

    *entry = physical;
}

void mmu_page(uint32_t virt, uint32_t phys, uint32_t flags, uint32_t second_base) {

    uint32_t offset = virt >> 20;
    uint32_t *entry = (uint32_t *) (MMU_TABLE_BASE | (offset << 2));

    // mask lower 20 bits of physical address, then add flags and 0x01 (coarse)
    uint32_t entry_val = (phys & 0xfffffc00) | (flags & 0xf0) | 0x01;

    // set firt level descriptor
    *entry = entry_val;

    // mask everything except bits 12-19
    offset = (virt >> 12) & 0xff;

    // create second level
    uint32_t *second_lvl_entry = (uint32_t *) ((second_base & 0xfffffc00) | (offset << 0x02));

    /*
     * form value of second level descriptor
     * 31:12 - page base address
     */
    uint32_t physical = (phys & 0xfffff000) | 0xff0 | (flags & 0xc) | 0x02;

    // set second level descriptor
    second_lvl_entry = physical;
}
