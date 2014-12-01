/** @file ctx_switch.c
 * 
 * @brief C wrappers around assembly context switch routines.
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-11-21
 */
 

#include <types.h>
#include <assert.h>

#include <config.h>
#include <kernel.h>
#include "sched_i.h"

#ifdef DEBUG_MUTEX
#include <exports.h>
#endif

static tcb_t* cur_tcb; /* use this if needed */

/**
 * @brief Initialize the current TCB and priority.
 *
 * Set the initialization thread's priority to IDLE so that anything
 * will preempt it when dispatching the first task.
 */
void dispatch_init(tcb_t* idle)
{
	cur_tcb = idle;
}


/**
 * @brief Context switch to the highest priority task while saving off the 
 * current task state.
 *
 * This function needs to be externally synchronized.
 * We could be switching from the idle task.  The priority searcher has been tuned
 * to return IDLE_PRIO for a completely empty run_queue case.
 */
void dispatch_save(void)
{
	// retrieve the highest priority task
	uint8_t prio = highest_prio();
	tcb_t* highest_tcb = runqueue_remove(prio);

	//if (cur_tcb == NULL || prio > cur_tcb->cur_prio) {
		//add saved task into run queue
	runqueue_add(cur_tcb, cur_tcb->cur_prio);
	ctx_switch_full((void*) &highest_tcb->context, (void*) &cur_tcb->context);

	//	runqueue_add(highest_tcb, prio);
	//}
}

/**
 * @brief Context switch to the highest priority task that is not this task -- 
 * don't save the current task state.
 *
 * There is always an idle task to switch to.
 */
void dispatch_nosave(void)
{
	// retrieve the highest priority tashk
	uint8_t prio = highest_prio();
	tcb_t* highest_tcb = runqueue_remove(prio);
	cur_tcb = highest_tcb;

	// set current registers to that of task we're switching to
	ctx_switch_half((void*) &highest_tcb->context);
}


/**
 * @brief Context switch to the highest priority task that is not this task -- 
 * and save the current task but don't mark is runnable.
 *
 * There is always an idle task to switch to.
 */
void dispatch_sleep(void)
{
	uint8_t prio = highest_prio();
	tcb_t* highest_tcb = runqueue_remove(prio);
	ctx_switch_full((void*) &highest_tcb->context, (void*)&cur_tcb->context);
}

/**
 * @brief Returns the priority value of the current task.
 */
uint8_t get_cur_prio(void)
{
	return cur_tcb->cur_prio;
}

/**
 * @brief Returns the TCB of the current task.
 */
tcb_t* get_cur_tcb(void)
{
	return cur_tcb;
}
