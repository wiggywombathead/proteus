.section .text

.global __aeabi_uidivmod

/**
 * __aeabi_uidivmod(u32 num, u32 den)
 * returns num % denom
 */
__aeabi_uidivmod:
    loop:
        cmp r0, r1
        blo return
        sub r0, r1
        b loop
    return:
        bx lr
