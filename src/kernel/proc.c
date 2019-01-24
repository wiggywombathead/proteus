#include <kernel/proc.h>
#include <kernel/interrupt.h>
#include <kernel/memory.h>
#include <kernel/mutex.h>
#include <kernel/spinlock.h>
#include <kernel/timer.h>

#include <common/stdlib.h>

extern uint8_t __end;
extern void switch_context(struct proc *old, struct proc *new);
extern int try_lock(spinlock_t *);

IMPLEMENT_LIST(proc);

struct proc_list all_procs;
struct proc_list ready_queue;

struct proc *current_process;
static uint32_t current_pid;

void proc_init(void) {
    struct proc *main;

    INIT_LIST(ready_queue);
    INIT_LIST(all_procs);

    main = kmalloc(sizeof(struct proc));
    main->stack_page = (void *) &__end;
    main->pid = current_pid++;
    strcpy(main->name, "init");

    append_proc_list(&all_procs, main);

    /* TODO: why does uncommenting this break scheduling */
    // current_process = main;

    timer_set(10000);
}

void schedule(void) {

    DISABLE_INTERRUPTS();
    struct proc *old_thread, *new_thread;

    /* if no other processes ready, just continue */
    if (size_proc_list(&ready_queue) == 0) {
        timer_set(10000);
        ENABLE_INTERRUPTS();
        return;
    }

    new_thread = dequeue_proc_list(&ready_queue);
    old_thread = current_process;
    current_process = new_thread;

    append_proc_list(&ready_queue, old_thread);

    switch_context(old_thread, new_thread);
    ENABLE_INTERRUPTS();
}

static void cleanup(void) {
    DISABLE_INTERRUPTS();
    struct proc *old_thread, *new_thread;

    while (size_proc_list(&ready_queue) == 0)
        ;

    new_thread = dequeue_proc_list(&ready_queue);
    old_thread = current_process;
    current_process = new_thread;

    free_page(old_thread->stack_page);
    kfree(old_thread);

    switch_context(old_thread, new_thread);
}

void create_kthread(kthreadfn func, char *name, int name_len) {
    struct proc *pcb;
    struct proc_state *new_state;

    pcb = kmalloc(sizeof(struct proc));
    pcb->pid = current_pid++;
    pcb->stack_page = alloc_page();
    strncpy(pcb->name, name, name_len < 20 ? name_len : 19);

    new_state = pcb->stack_page + PAGE_SIZE - sizeof(struct proc_state);
    pcb->state = new_state;

    bzero(new_state, sizeof(struct proc_state));
    new_state->lr = (uint32_t) func;
    new_state->sp = (uint32_t) cleanup;
    new_state->cpsr = 0x13 | (8 << 1);

    append_proc_list(&all_procs, pcb);
    append_proc_list(&ready_queue, pcb);
}

void spin_init(spinlock_t *lock) {
    *lock = 1;
}

/* keep checking if lock free */
void spin_lock(spinlock_t *lock) {
    while (!try_lock(lock))
        ;
}

/* acquire the lock */
void spin_unlock(spinlock_t *lock) {
    *lock = 1;
}
void mutex_init(struct mutex *mutex) {
    mutex->lock = 1;
    mutex->locker = 0;
    INIT_LIST(mutex->wait_queue);
}

void mutex_lock(struct mutex *mutex) {
    struct proc *old_thread, *new_thread;

    /* if lock unavailable, stop execution and go to wait queue */
    while (!try_lock(&mutex->lock)) {
        DISABLE_INTERRUPTS();
        new_thread = dequeue_proc_list(&ready_queue);
        old_thread = current_process;
        current_process = new_thread;

        append_proc_list(&mutex->wait_queue, old_thread);

        /* perform context switch */
        switch_context(old_thread, new_thread);
        ENABLE_INTERRUPTS();
    }

    mutex->locker = current_process;
}

void mutex_unlock(struct mutex *mutex) {
    struct proc *proc;

    mutex->lock = 1;
    mutex->locker = 0;

    /* if a process was waiting for this, put them on the run queue */
    if (size_proc_list(&mutex->wait_queue)) {
        proc = dequeue_proc_list(&mutex->wait_queue);
        prepend_proc_list(&ready_queue, proc);
    }
}
