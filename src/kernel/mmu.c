#include <kernel/mmu.h>
#include <kernel/peripheral.h>

void mmu_init(void) {

    /* make RAM available - 512MiB => 0x20000000 in 1MiB sections */
    for (uint32_t i = 0x00000000; i < PERIPHERAL_BASE; i += 0x00100000)
        mmu_section(i, i, BUFFERABLE | CACHEABLE);

    /* make peripherals available - DO NOT CACHE! */
    for (uint32_t i = PERIPHERAL_BASE; i < PERIPHERAL_END; i += 0x00100000)
        mmu_section(i, i, 0x0000);

    mmu_start(MMU_TTABLE_BASE);

}

/**
 * Declare a 1MiB section in the MMU
 */
void mmu_section(uint32_t virt, uint32_t phys, uint32_t flags) {

    // take top 12 bits
    uint32_t offset = virt >> 20;

    // multiply by 4 and add to TLB base
    uint32_t *entry = (uint32_t *) (MMU_TTABLE_BASE | (offset << 2));

    // mask lower 20 bits of physical address, then add flags and 0x02 (1MiB)
    uint32_t physical = (phys & 0xfff00000) | (flags & 0x7ffa) | 0x02;

    *entry = physical;
}

/**
 * Declare a 4KiB "small page" section in the MMU
 */
void mmu_page(uint32_t virt, uint32_t phys, uint32_t flags, uint32_t second_base) {

    uint32_t offset = virt >> 20;
    uint32_t *entry = (uint32_t *) (MMU_TTABLE_BASE | (offset << 2));

    // mask lower 20 bits of physical address, then add flags and 0x01 (4KiB)
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
