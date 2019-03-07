#include <kernel/sched.h>
#include <kernel/interrupt.h>
#include <kernel/proc.h>
#include <kernel/timer.h>

extern void switch_context(struct proc *old, struct proc *new);

extern struct proc_list job_queue;
extern struct proc_list ready_queue;

extern struct proc *current_process;

/* set the scheduling algorithm */
void sched_init(void) {

#if defined ( SCHED_FCFS )
    schedule = sched_fcfs;
#elif defined ( SCHED_ROUNDROBIN )
    schedule = sched_round_robin;
#else
    schedule = sched_round_robin;
#endif

}

void sched_round_robin(void) {

    DISABLE_INTERRUPTS();

    struct proc *old_thread, *new_thread;

    if (size_proc_list(&ready_queue) == 0) {
        timer_set(QUANTUM);
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
