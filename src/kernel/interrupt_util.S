.section .text

.global interrupts_enabled
interrupts_enabled:
	mrs r1, cpsr
	asr r1, r1, #7
	and r1, r1, #1
	cmp r1, #0
	beq 1f

	mov r0, #0
	bx lr

	1:
		mov r0, #1
		bx lr

.global enable_interrupts
enable_interrupts:
	/*
	bl interrupts_enabled
	cmp r0, #0
	beq not_enabled
	bx lr

	not_enabled:
	 */
	cpsie i
	bx lr

.global disable_interrupts
disable_interrupts:
	/*
	bl interrupts_enabled
	cmp r0, #1
	blt enabled
	bx lr

	enabled:
	 */
	cpsid i
	bx lr
