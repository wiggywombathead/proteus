.section .text

.global __aeabi_uidivmod

/**
 * __aeabi_uidivmod(u32 num, u32 den)
 * returns num % denom
 */
__aeabi_uidivmod:
    mov r2, #0  // quotient
    loop:
        cmp r0, r1
        blo return
        sub r0, r0, r1
        add r2, r2, #1
        b loop
    return:
        mov r1, r0
        mov r0, r2
        bx lr
