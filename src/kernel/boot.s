.section ".text.boot"

.global _start  /* make _start visible outside file */

/* 
 * kernel entry point
 *  r0:  0x00000000
 *  r1:  0x00000c42
 *  r2:  0x00000100 = start of atags
 *  r15: begins execution at 0x8000
 */
_start:

    mrc p15, #0, r1, c0, c0, #5
    and r1, r1, #3
    cmp r1, #0
    bne halt

    /* setup the stack */
    mov sp, #0x8000

    /* clear bss */
    ldr r4, =__bss_start
    ldr r9, =__bss_end

    mov r5, #0
    mov r6, #0
    mov r7, #0
    mov r8, #0
    b 2f

1:
    stmia r4!, {r5-r8}
    /* loop if we are still below __bss_end */

2:
    cmp r4, r9
    blo 1b

    /* call kernel_main */
    ldr r3, =kernel_main
    blx r3

halt:
    wfe
    b halt
