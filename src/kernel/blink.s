.globl act_blink

/* 
 *  void blink(uint32_t n)
 *      blink ACT LED n times
 */
act_blink:
    /* check passed value non-negative */
    cmp r0, #0
    blt halt

    /* store address to resume execution after */
    push {lr}
    
    ldr r1, =0x3f200000

    /* select GPIO function 4 */
    mov r2, #1
    lsl r2, #21
    str r2, [r1, #0x10]

    /* want to set/clear pin 47 = 32 + 15 */
    mov r2, #1
    lsl r2, #15

loop$:
    /* blink while n > 0 */
    cmp r0, #0
    beq halt

    /* write to GPSET1 */
    str r2, [r1, #0x20]
    mov r3, #0x1f0000

wait1$:
    sub r3, #1
    cmp r3, #0
    bne wait1$

    /* write to GPCLR1 */
    str r2, [r1, #0x2c]
    mov r3, #0x1f0000

wait2$:
    sub r3, #1
    cmp r3, #0
    bne wait2$

    sub r0, #1
    b loop$

halt:
    /* exit function */
    pop {pc}
