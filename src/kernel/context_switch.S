.section .rodata

#include <kernel/sched.h>

.section .text

.global switch_context

/*
 * switch_context(struct proc *old, struct proc *new)
 *     give control of CPU from old to new
 *     old thread must be put back in queue before calling
 */

switch_context:
	/* save current process' state */
	push {lr}
	push {sp}
	mrs r12, cpsr   /* get current status register */
	push {r0-r12}   /* save general purpose regs and state */
	str sp, [r0]    /* store stack pointer in the state field of proc */

	/* load new process' state */
	ldr sp, [r1]
	ldr r0, =QUANTUM    /* quantum time of 20ms */
	bl timer_set        /* set timer to go off in one quantum */
	pop {r0-r12}
	msr cpsr_c, r12
	pop {lr, pc}
