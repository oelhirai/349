/** @file proc.c
 * 
 * @brief Implementation of `process' syscalls
 *
 * @author Mike Kasick <mkasick@andrew.cmu.edu>
 * @date   Sun, 14 Oct 2007 00:07:38 -0400
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-11-12
 */

#include <exports.h>
#include <bits/errno.h>
#include <config.h>
#include <kernel.h>
#include <syscall.h>
#include <sched.h>

#include <arm/reg.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/physmem.h>
#include <device.h>

int task_create(task_t* tasks , size_t num_tasks)
{
	task_t* sortedTasks[num_tasks];
	size_t i, j;
	for(i = 0; i < num_tasks; i ++)
	{
		sortedTasks[i] = &(tasks[i]);
	}
	for(i = 0; i < num_tasks; i++)
	{
		for(j = i + 1; j < num_tasks; j++)
		{
			if(sortedTasks[i]->T > sortedTasks[j]->T)
			{
				task_t* temp = sortedTasks[j];
				sortedTasks[j] = sortedTasks[i];
				sortedTasks[i] = temp;
			}
		}
	}
	allocate_tasks(sortedTasks, num_tasks);
	return 1; /* remove this line after adding your code */
}

int event_wait(unsigned int dev)
{
	dev_wait(dev);
	// Add stuff to start next task
	dispatch_sleep();
	return 1; /* remove this line after adding your code */	
}

/* An invalid syscall causes the kernel to exit. */
void invalid_syscall(unsigned int call_num  __attribute__((unused)))
{
	printf("Kernel panic: invalid syscall -- 0x%08x\n", call_num);

	disable_interrupts();
	while(1);
}
