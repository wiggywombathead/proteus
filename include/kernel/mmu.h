#ifndef _MMU_H
#define _MMU_H

#include <stdint.h>

#define MMU_TTABLE_BASE 0x00004000

#define BUFFERABLE  0x04
#define CACHEABLE   0x08

extern void mmu_start(uint32_t);
extern void mmu_stop(void);
extern void tlb_invalidate(void);

void mmu_init(void);
void mmu_section(uint32_t virt, uint32_t phys, uint32_t flags);
void mmu_page(uint32_t virt, uint32_t phys, uint32_t flags, uint32_t second);

#endif
