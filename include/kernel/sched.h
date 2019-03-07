#ifndef _SCHED_H
#define _SCHED_H

#define QUANTUM 20000

typedef void (*schedulerfn)(void);

void sched_init(void);
void schedule(void);

/* scheduling algorithms */
void sched_round_robin(void);
void sched_fcfs(void);

#endif
