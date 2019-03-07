#ifndef _SCHED_H
#define _SCHED_H

#define QUANTUM 20000

/* scheduler to use */
void (*schedule)(void);

void sched_init(void);

/* scheduling algorithms */
void sched_round_robin(void);
void sched_fcfs(void);

#endif
