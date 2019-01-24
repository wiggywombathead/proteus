#ifndef _SPINLOCK_H
#define _SPINLOCK_H

typedef int spinlock_t;

void spin_init(spinlock_t *);
void spin_lock(spinlock_t *);
void spin_unlock(spinlock_t *);

#endif
