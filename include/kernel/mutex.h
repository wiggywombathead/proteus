#ifndef _MUTEX_H
#define _MUTEX_H

#include <kernel/proc.h>

struct mutex {
    int lock;
    struct proc *locker;    // process that locked lock
    struct proc_list wait_queue;
};

typedef struct mutex mutex_t;

void mutex_init(struct mutex *);
void mutex_lock(struct mutex *);
void mutex_unlock(struct mutex *);

#endif
