#ifndef _PROC_H
#define _PROC_H

#include <stdint.h>
#include <kernel/list.h>

struct proc_state {
    uint32_t r0;
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
    uint32_t r4;
    uint32_t r5;
    uint32_t r6;
    uint32_t r7;
    uint32_t r8;
    uint32_t r9;
    uint32_t r10;
    uint32_t r11;
    uint32_t cpsr;
    uint32_t sp;
    uint32_t lr;
};

DEFINE_LIST(proc);

/* process control block definition */
struct proc {
    struct proc_state *state;
    uint32_t pid;
    char name[32];
    void *stack_page;
    DEFINE_LINK(proc);
};

typedef void (*kthreadfn)(void);

void proc_init(void);
void schedule(void);
void create_kthread(kthreadfn, char *, int);

/* scheduling algorithm */
typedef void (*schedulerfn)(void);

void sched_round_robin(void);
void sched_fcfs(void);

#endif
