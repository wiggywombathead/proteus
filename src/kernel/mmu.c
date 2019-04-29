#include <kernel/mmu.h>
#include <kernel/gpio.h>
#include <kernel/peripheral.h>
#include <common/stdio.h>

/**
 * Initialise the Memory Management Unit
 */
void mmu_init(void) {

    for (uint32_t i = 0x0; ; i += 0x00100000) {
        mmu_section(i, i, 0x0000);
        if (i == 0xfff00000) break;
    }

    mmu_start(MMU_TTABLE_BASE, 0x1000|0x4|0x1);
}

/**
 * Declare a 1MiB section in the MMU
 * @param virt Virtual address to map to
 * @param phys Physical address to be mapped
 * @param flags Flags (BUFFERABLE, CACHEABLE)
 */
void mmu_section(uint32_t virt, uint32_t phys, uint32_t flags) {

    // take top 12 bits
    uint32_t offset = virt >> 20;

    // multiply by 4 and add to TLB base
    uint32_t entry = MMU_TTABLE_BASE | (offset << 2);

    // mask lower 20 bits of physical address, then add flags and 0x02 (1MiB)
    uint32_t physical = (phys & 0xfff00000) | 0xc00 | flags | 0x02;

    mmio_write(entry, physical);
}

/**
 * Declare a 4KiB "small page" section in the MMU
 * @param virt Virtual address to map to
 * @param phys Physical address to be mapped
 * @param flags Flags (BUFFERABLE, CACHEABLE)
 * @param second_base Address of second-level descriptor
 */
void mmu_page(uint32_t virt, uint32_t phys, uint32_t flags, uint32_t second_base) {

    uint32_t offset = virt >> 20;
    uint32_t entry = MMU_TTABLE_BASE | (offset << 2);

    uint32_t physical = (second_base & 0xfffffc00) | 1;

    mmio_write(entry, physical);

    offset = (virt >> 12) & 0xff;
    entry = (second_base & 0xfffffc00) & (offset << 2);
    physical = (phys & 0xfffff000) | 0xff0 | flags | 0x2;

    mmio_write(entry, physical);

    /*
    // mask lower 20 bits of physical address, then add flags and 0x01 (4KiB)
    uint32_t entry_val = (phys & 0xfffffc00) | (flags & 0xf0) | 0x01;

    // set firt level descriptor
    *entry = entry_val;

    // mask everything except bits 12-19
    offset = (virt >> 12) & 0xff;

    // create second level
    uint32_t *second_lvl_entry = (uint32_t *) ((second_base & 0xfffffc00) | (offset << 0x02));

     form value of second level descriptor
     31:12 - page base address
    uint32_t physical = (phys & 0xfffff000) | 0xff0 | (flags & 0xc) | 0x02;

    // set second level descriptor
    second_lvl_entry = physical;
    */
}
