.section .text

/**
 * void mmu_init(uint32_t addr)
 * initialise MMU at address addr
 */
.global mmu_start
mmu_start:
    mov r1, #0

    mcr p15,0,r1,c7,c7,0    // invalidate caches
    mcr p15,0,r1,c8,c7,0    // invalidate TLB entries
    mcr p15,0,r1,c7,c10,4   // data sync barrier

    // set all domains to 0x3 == 0b11
    ldr r1, =0xffffffff
    mcr p15,0,r1,c3,c0,0

    // set translation table base address
    mcr p15,0,r0,c2,c0,0

    /* set bits 0,2,11,12,22 => 0b00000000010000000001100000000101 */
    ldr r1, =0x00401805
    mrc p15,0,r2,c1,c0,0
    orr r2, r2, r1
    mcr p15,0,r2,c1,c0,0

    mov pc, lr

.global mmu_stop
mmu_stop:
    mrc p15,0,r2,c1,c0,0
    bic r2, #0x1000
    bic r2, #0x0004
    bic r2, #0x0001
    mcr p15,0,r2,c1,c0,0
    bx lr

.global tlb_invalidate
tlb_invalidate:
    mov r2, #0
    mcr p15,0,r2,c8,c7,0    // invalidate tlb
    mcr p15,0,r2,c7,c10,4   // data sync barrier
    bx lr
