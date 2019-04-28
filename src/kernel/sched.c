#include <kernel/sched.h>
#include <kernel/interrupt.h>
#include <kernel/proc.h>
#include <kernel/timer.h>

#include <common/stdio.h>
#include <common/stdlib.h>
#include <common/string.h>

extern void switch_context(struct proc *old, struct proc *new);

extern struct proc_list job_queue;
extern struct proc_list ready_queue;

extern struct proc *current_process;

/**
 * @brief Define which scheduling algorithm to use
 *
 * Uses Make's directives supplied at compile-time to specify the option of
 * scheduling algorithm. The function pointer schedule is set using these
 * directives.
 */
void sched_init(void) {

    char sch_str[16];

#if defined ( SCHED_FCFS )
    schedule = sched_fcfs;
    strcpy(sch_str, "FCFS");
#elif defined ( SCHED_ROUNDROBIN )
    schedule = sched_round_robin;
    strcpy(sch_str, "Round Robin");
#else
    schedule = sched_round_robin;
    strcpy(sch_str, "Round Robin");
#endif

    printf("\tScheduler: %s\n", sch_str);
}

/**
 * @brief Implementation of the round robin scheduling algorithm
 */
void sched_round_robin(void) {

    DISABLE_INTERRUPTS();

    struct proc *old_thread, *new_thread;

    if (size_proc_list(&ready_queue) == 0) {
        timer_set(QUANTUM);
        // enable_interrupts();
        ENABLE_INTERRUPTS();
        return;
    }

    new_thread = dequeue_proc_list(&ready_queue);
    old_thread = current_process;
    current_process = new_thread;

    append_proc_list(&ready_queue, old_thread);

    switch_context(old_thread, new_thread);

    // enable_interrupts();
    ENABLE_INTERRUPTS();
}

/**
 * @brief Implementation of the First Come First Served scheduling algorithm
 */
void sched_fcfs(void) {
    while (1)
        ;
}
