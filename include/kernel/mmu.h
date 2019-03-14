#ifndef _MMU_H
#define _MMU_H

#include <stdint.h>

extern void mmu_init(void);
extern void mmu_stop(void);
extern void tlb_invalidate(void);

void mmu_entry(uint32_t virt, uint32_t phys, uint32_t flags);
void mmu_page(uint32_t virt, uint32_t phys, uint32_t flags, uint32_t second);

#endif
