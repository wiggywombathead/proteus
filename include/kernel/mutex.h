#ifndef _MUTEX_H
#define _MUTEX_H

#include <kernel/proc.h>

struct mutex {
    int lock;
    struct proc *locker;    // process that locked lock
    struct proc_list wait_queue;
};

typedef struct mutex mutex_t;

void mutex_init(mutex_t *);
void mutex_lock(mutex_t *);
void mutex_unlock(mutex_t *);

#endif
