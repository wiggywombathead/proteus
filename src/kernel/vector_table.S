.section .text

.global move_exception_vector

/* match layout of Interrupt Vector Table */
exception_vector:
    /* load absolute addresses of handlers */
    ldr pc, _reset_h
    ldr pc, _undefined_instruction_h
    ldr pc, _software_interrupt_h
    ldr pc, _prefetch_abort_h
    ldr pc, _data_abort_h
    nop /* reserved */
    ldr pc, _irq_h
    ldr pc, _fiq_h

_reset_h:                   .word reset_handler
_undefined_instruction_h:   .word undefined_instruction_handler
_software_interrupt_h:      .word software_interrupt_handler
_prefetch_abort_h:          .word prefetch_abort_handler
_data_abort_h:              .word data_abort_wrapper
_irq_h:                     .word irq_handler_wrapper
_fiq_h:                     .word fiq_handler

move_exception_vector:
    push {r4-r9}
    ldr r0, =exception_vector
    mov r1, #0x0000
    ldmia r0!, {r2-r9}
    stmia r1!, {r2-r9}
    ldmia r0!, {r2-r8}
    stmia r1!, {r2-r8}
    pop {r4-r9}
    blx lr

data_abort_wrapper:
    mov r7, r0
    mov r0, #0x10

    mov r6, lr
    ldr r8, [r6, #-8]
    mrc p15,0,r4,c5,c0,0
    mrc p15,0,r5,c5,c0,1

    mov sp, #0x00004000
    mov r0, r5
    bl data_abort_handler

irq_handler_wrapper:
    sub lr, lr, #4
    srsdb sp!, #0x13
    cpsid if, #0x13
    push {r0-r3, r12, lr}
    and r1, sp, #4
    sub sp, sp, r1
    push {r1}
    bl irq_handler
    pop {r1}
    add sp, sp, r1
    pop {r0-r3, r12, lr}
    rfeia sp!
