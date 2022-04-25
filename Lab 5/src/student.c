/*
 * student.c
 * Multithreaded OS Simulation for CS 2200 and ECE 3058
 *
 * This file contains the CPU scheduler for the simulation.
 */

#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "os-sim.h"

/** Function prototypes **/
extern void idle(unsigned int cpu_id);
extern void preempt(unsigned int cpu_id);
extern void yield(unsigned int cpu_id);
extern void terminate(unsigned int cpu_id);
extern void wake_up(pcb_t *process);


/*
 * current[] is an array of pointers to the currently running processes.
 * There is one array element corresponding to each CPU in the simulation.
 *
 * current[] should be updated by schedule() each time a process is scheduled
 * on a CPU.  Since the current[] array is accessed by multiple threads, you
 * will need to use a mutex to protect it.  current_mutex has been provided
 * for your use.
 */
static pcb_t **current;
static pthread_mutex_t current_mutex;

typedef struct DLL { 
    pcb_t* head;
    pcb_t* tail;
    int size;
} DLL;

//These all need to be init right?
static pthread_mutex_t ready_mutex;
static pthread_cond_t cond_NE;
static int condition;
static pthread_mutex_t ready_mutex_NE;
static DLL* process_queue;

static void init_queue() {
    printf("Init: \n");
    process_queue = (DLL*) malloc(sizeof(DLL));
    process_queue->head = NULL;
    process_queue->tail = NULL;
    process_queue->size = 0;
    condition = 0;
}

static pcb_t* pop_process() {
    pthread_mutex_lock(&ready_mutex);
    if (process_queue->size == 0) {
        printf("Popped: NULL");
        pthread_mutex_unlock(&ready_mutex);
        return NULL;
    } 
    pcb_t* node = process_queue->head;
    if (process_queue->size == 1) {
        process_queue->head = NULL;
        process_queue->tail = NULL;
        condition = 0;
    } else {
        process_queue->head = node->next;
    }
    process_queue->size--;
    pthread_mutex_unlock(&ready_mutex);
    printf("Popped: %s \n", node->name);
    return node;
}


static void add_node(pcb_t* node) {
    printf("Added: %s \n", node->name);
    pthread_mutex_lock(&ready_mutex);
    if (process_queue->size == 0) {
        process_queue->head = node;
        process_queue->tail = node;
    } else {
        pcb_t* prev = process_queue->tail;
        prev->next = node;
        process_queue->tail = node;
    }
    process_queue->size++;
    condition = 1;
    pthread_cond_signal(&cond_NE);
    pthread_mutex_unlock(&ready_mutex);
    printf("Added Last: %s \n", node->name);
}

/*
 * schedule() is your CPU scheduler.  It should perform the following tasks:
 *
 *   1. Select and remove a runnable process from your ready queue which 
 *	you will have to implement with a linked list or something of the sort.
 *
 *   2. Set the process state to RUNNING
 *
 *   3. Call context_switch(), to tell the simulator which process to execute
 *      next on the CPU.  If no process is runnable, call context_switch()
 *      with a pointer to NULL to select the idle process.
 *	The current array (see above) is how you access the currently running process indexed by the cpu id. 
 *	See above for full description.
 *	context_switch() is prototyped in os-sim.h. Look there for more information 
 *	about it and its parameters.
 */
static void schedule(unsigned int cpu_id)
{
    printf("Scheduled: %d \n", cpu_id);
    pcb_t* process = pop_process();
    if (process != NULL) {
        process->state = PROCESS_RUNNING;
    }
    pthread_mutex_lock(&current_mutex);
    current[cpu_id] = process;
    context_switch(cpu_id, process, -1);
    pthread_mutex_unlock(&current_mutex);

}


/*
 * idle() is your idle process.  It is called by the simulator when the idle
 * process is scheduled.
 *
 * This function should block until a process is added to your ready queue.
 * It should then call schedule() to select the process to run on the CPU.
 */
extern void idle(unsigned int cpu_id)
{
    printf("Idling: \n");
    pthread_mutex_lock(&ready_mutex);
    while(!condition) {
        pthread_cond_wait(&cond_NE, &ready_mutex_NE);
    }
    pthread_mutex_unlock(&ready_mutex);

    schedule(cpu_id);
}


/*
 * preempt() is the handler called by the simulator when a process is
 * preempted due to its timeslice expiring.
 *
 * This function should place the currently running process back in the
 * ready queue, and call schedule() to select a new runnable process.
 */
extern void preempt(unsigned int cpu_id)
{
    printf("Preempt: \n");
    pthread_mutex_lock(&current_mutex);
    pcb_t* current_process = current[cpu_id];
    pthread_mutex_unlock(&current_mutex);
    current_process->state = PROCESS_READY;
    add_node(current_process);
    schedule(cpu_id);
}


/*
 * yield() is the handler called by the simulator when a process yields the
 * CPU to perform an I/O request.
 *
 * It should mark the process as WAITING, then call schedule() to select
 * a new process for the CPU.
 */
extern void yield(unsigned int cpu_id)
{
    printf("Yield: \n");
    pthread_mutex_lock(&current_mutex);
    pcb_t* current_process = current[cpu_id];
    current_process->state = PROCESS_WAITING;
    pthread_mutex_unlock(&current_mutex);
    schedule(cpu_id);
}


/*
 * terminate() is the handler called by the simulator when a process completes.
 * It should mark the process as terminated, then call schedule() to select
 * a new process for the CPU.
 */
extern void terminate(unsigned int cpu_id)
{
    printf("Terminate: \n");
    pthread_mutex_lock(&current_mutex);
    pcb_t* current_process = current[cpu_id];
    current_process->state = PROCESS_TERMINATED;
    pthread_mutex_unlock(&current_mutex);
    schedule(cpu_id);
}


/*
 * wake_up() is the handler called by the simulator when a process's I/O
 * request completes.  It should perform the following tasks:
 *
 *   1. Mark the process as READY, and insert it into the ready queue.
 *
 *   2. If the scheduling algorithm is LRTF, wake_up() may need
 *      to preempt the CPU with lower remaining time left to allow it to
 *      execute the process which just woke up with higher reimaing time.
 * 	However, if any CPU is currently running idle,
* 	or all of the CPUs are running processes
 *      with a higher remaining time left than the one which just woke up, wake_up()
 *      should not preempt any CPUs.
 *	To preempt a process, use force_preempt(). Look in os-sim.h for 
 * 	its prototype and the parameters it takes in.
 */
extern void wake_up(pcb_t *process)
{
    printf("Wake: \n");
    process->state = PROCESS_READY;
    add_node(process);
}


/*
 * main() simply parses command line arguments, then calls start_simulator().
 * You will need to modify it to support the -l and -r command-line parameters.
 */
int main(int argc, char *argv[])
{
    unsigned int cpu_count;

    /* Parse command-line arguments */
    if (argc != 2)
    {
        fprintf(stderr, "Multithreaded OS Simulator\n"
            "Usage: ./os-sim <# CPUs> [ -l | -r <time slice> ]\n"
            "    Default : FIFO Scheduler\n"
	    "         -l : Longest Remaining Time First Scheduler\n"
            "         -r : Round-Robin Scheduler\n\n");
        return -1;
    }

    //init process queue
    init_queue();


    cpu_count = strtoul(argv[1], NULL, 0);

    /* FIX ME - Add support for -l and -r parameters*/

    /* Allocate the current[] array and its mutex */
    current = malloc(sizeof(pcb_t*) * cpu_count);
    assert(current != NULL);
    pthread_mutex_init(&current_mutex, NULL);

    /* Start the simulator in the library */
    start_simulator(cpu_count);

    //free process queue at the end of simulation
    free(process_queue);
    return 0;
}


