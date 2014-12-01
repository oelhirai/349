/** @file sched.c
 * 
 * @brief Top level implementation of the scheduler.
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-11-20
 */

#include <types.h>
#include <assert.h>

#include <kernel.h>
#include <config.h>
#include "sched_i.h"

#include <arm/reg.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/physmem.h>

tcb_t system_tcb[OS_MAX_TASKS]; /*allocate memory for system TCBs */

// Probably dont need this
void sched_init(task_t* main_task  __attribute__((unused)))
{
	
}

/**
 * @brief This is the idle task that the system runs when no other task is runnable
 */
 
static void __attribute__((unused)) idle(void)
{
	 enable_interrupts();
	 while(1);
}


void setDefaultContext(sched_context_t context, void* sp, void* data, task_fun_t task, uint32_t kstack_high)
{
	// set up default arguments for new tasks
	context->r4 = (void*) task;
	context->r5 = data;
	context->r6 = sp;
	context->r7 = 0;
	context->r8 = 0;
	context->r9 = 0;
	context->r10 = 0;
	context->r11 = 0;
	context->sp = (void *)kstack_high;
	context->lr = 0xCAFEBABE;
}
/**
 * @brief Allocate user-stacks and initializes the kernel contexts of the
 * given threads.
 *
 * This function assumes that:
 * - num_tasks < number of tasks allowed on the system.
 * - the tasks have already been deemed schedulable and have been appropriately
 *   scheduled.  In particular, this means that the task list is sorted in order
 *   of priority -- higher priority tasks come first.
 *
 * @param tasks  A list of scheduled task descriptors.
 * @param size   The number of tasks is the list.
 */
void allocate_tasks(task_t** tasks  __attribute__((unused)), size_t num_tasks  __attribute__((unused)))
{
	// They are sent sorted
	int i;
	for(i = 1; i < num_tasks; i++)
	{
		system_tcb[i]->native_prio = i;
		system_tcb[i]->cur_prio = i;
		void* sp = (*tasks)->stack_pos;
		void* data = (*tasks)->data;
		task_fun_t task = (*tasks)->lambda;
		setDefaultContext(system_tcb[i]->context, sp, data, task, system_tcb[i]->kstack_high);
		tasks++
	}
	// Setup idle task
	system_tcb[i]->native_prio = i;
	system_tcb[i]->cur_prio = i;
	setDefaultContext(system_tcb[i]->context, 0, 0, idle, system_tcb[i]->kstack_high);

	// begin running idle task
	
	runqueue_init();
	for(i = 1; i < num_tasks + 1; i++)
	{
		runqueue_add(system_tcb[i], i);
	}
	ctx_switch_half(system_tcb[1]->context);
}

