/**
 * @file sched.h
 * @brief File containing all scheduling algorithms available to the system
 */

#ifndef _SCHED_H
#define _SCHED_H

#define QUANTUM 20000

#if !defined(__ASSEMBLER__)
/* scheduler to use */
void (*schedule)(void);

void sched_init(void);

/* scheduling algorithms */
void sched_round_robin(void);
void sched_fcfs(void);
#endif /* __ASSEMBLER__ */

#endif
