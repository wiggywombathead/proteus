#include <kernel/proc.h>
#include <kernel/interrupt.h>
#include <kernel/memory.h>
#include <kernel/mutex.h>
#include <kernel/sched.h>
#include <kernel/spinlock.h>
#include <kernel/timer.h>

#include <common/stdlib.h>
#include <common/string.h>

extern uint8_t __end;
extern void switch_context(struct proc *old, struct proc *new);
extern int try_lock(spinlock_t *);

IMPLEMENT_LIST(proc);

struct proc_list job_queue;
struct proc_list ready_queue;

struct proc *current_process;

static uint32_t current_pid = 1;

/**
 * @brief Initialse ready queue and job queue, and start the init process
 *
 * The init process is created and added to the job queue, the current process
 * as this process, and the timer is set to go off in one quantum.
 */
void proc_init(void) {

	struct proc *init;

	INIT_LIST(ready_queue);
	INIT_LIST(job_queue);

	init = kmalloc(sizeof(struct proc));
	init->stack_page = (void *) &__end;
	init->pid = current_pid++;
	strcpy(init->name, "init");

	append_proc_list(&job_queue, init);
	append_proc_list(&ready_queue, init);

	current_process = init;

	timer_set(QUANTUM);
}

/**
 * @brief Clean up after a process terminates
 *
 * This takes a process which has just finished executing and frees all of its
 * resources. It then removes it from the ready queue and job queue and switches
 * context so that the next ready process may execute.
 */
static void cleanup(void) {

	disable_interrupts();

	struct proc *old_thread, *new_thread;

	while (size_proc_list(&ready_queue) == 0)
		;

	new_thread = dequeue_proc_list(&ready_queue);
	old_thread = current_process;
	current_process = new_thread;

	/* free resources used by process */
	free_page(old_thread->stack_page);
	kfree(old_thread);

	remove_proc_list(&job_queue, old_thread);
	remove_proc_list(&ready_queue, old_thread);

	/* context switch */
	switch_context(old_thread, new_thread);
}

/**
 * Create a new process
 * @param func Name of function the process will execute
 * @param name Name of the process
 *
 * This creates a new thread of execution by creating a new instance of a
 * process control block in a new stack page. It is then added to the job queue
 * and ready queue.
 */
void create_kthread(kthreadfn func, char *name) {
	struct proc *pcb;
	struct cpu_state *new_state;

	pcb = kmalloc(sizeof(struct proc));
	pcb->pid = current_pid++;
	pcb->stack_page = alloc_page();

	// size_t len = strlen(name);
	strncpy(pcb->name, name, 32);

	new_state = pcb->stack_page + PAGE_SIZE - sizeof(struct cpu_state);
	pcb->state = new_state;

	bzero(new_state, sizeof(struct cpu_state));
	new_state->lr = (uint32_t) func;
	new_state->sp = (uint32_t) cleanup;
	new_state->cpsr = 0x13;

	append_proc_list(&job_queue, pcb);
	append_proc_list(&ready_queue, pcb);
}

/**
 * Initialise a spinlock
 * @param lock Address to initialise as lock variable
 */
void spin_init(spinlock_t *lock) {
	*lock = 1;
}

/**
 * Busy-wait poll the lock
 * @param lock Lock variable to poll
 */
void spin_lock(spinlock_t *lock) {
	while (!try_lock(lock))
		;
}

/**
 * Acquire the lock variable
 * @param lock Lock to acquire
 */
void spin_unlock(spinlock_t *lock) {
	*lock = 1;
}

/**
 * Initialise a mutual exclusion lock by setting the process that locked it and
 * initialising its wait queue
 * @param mutex Pointer to a mutex lock to initialise
 */
void mutex_init(struct mutex *mutex) {
	mutex->lock = 1;
	mutex->locker = 0;
	INIT_LIST(mutex->wait_queue);
}

/**
 * Lock the mutex
 * @param mutex Mutex to lock
 */
void mutex_lock(mutex_t *mutex) {
	struct proc *old_thread, *new_thread;

	/* if lock unavailable, stop execution and go to wait queue */
	while (!try_lock(&mutex->lock)) {

		disable_interrupts();

		new_thread = dequeue_proc_list(&ready_queue);
		old_thread = current_process;
		current_process = new_thread;

		append_proc_list(&mutex->wait_queue, old_thread);

		/* perform context switch */
		switch_context(old_thread, new_thread);

		enable_interrupts();
	}

	mutex->locker = current_process;
}

/**
 * Unlock a mutex
 * @param mutex Mutex to unlock
 */
void mutex_unlock(mutex_t *mutex) {
	struct proc *proc;

	mutex->lock = 1;
	mutex->locker = 0;

	/* if a process was waiting for this, put them on the run queue */
	if (size_proc_list(&mutex->wait_queue)) {
		proc = dequeue_proc_list(&mutex->wait_queue);
		prepend_proc_list(&ready_queue, proc);
	}
}
