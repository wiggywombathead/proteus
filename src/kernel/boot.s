.section ".text.boot"

.global _start  /* make _start visible outside file */

/* 
 * kernel entry point
 *  r0:  0x00000000
 *  r1:  0x000012dc
 *  r2:  0x00000100 = start of atags
 *  r15: begins execution at 0x8000
 */
_start:

    /* shutdown three cores */
    mrc p15, #0, r1, c0, c0, #5
    and r1, r1, #3
    cmp r1, #0
    bne halt

    /* setup the stack */
    mov sp, #0x8000

    /* setup bss */
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
    blo 1b;

    /* begin ACT LED code */
    ldr r0, =0x3f200000

    mov r1, #1
    lsl r1, #21
    str r1, [r0, #0x10]     @ set GPIO function select
    mov r1, #1
    lsl r1, #15             @ set 15th bit of r1

loop$:
    str r1, [r0, #0x20]     @ set GPIO 15 high, turning LED on
    mov r2, #0x1f0000       @ set counter variable for delay

wait1$:
    sub r2, #1
    cmp r2, #0
    bne wait1$

    str r1, [r0, #0x2c]     @ set GPIO 15 low, turning LED off

    mov r2, #0x1f0000

wait2$:
    sub r2, #1
    cmp r2, #0
    bne wait2$

    b loop$

    // call kernel_main
    ldr r3, =kernel_main
    blx r3

halt:
    wfe     /* enter low power state */
    b halt
