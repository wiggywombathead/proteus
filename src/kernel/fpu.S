.section .text

.global fpu_enable
/**
 * void enable_fpu(void)
 */
fpu_enable:
	ldr r0, =(0xf << 20)
	mcr p15, 0, r0, c1, c0, 2

	// enable FPU in floating-point exception register
	mov r3, #0x40000000
	vmsr fpexc, r3
