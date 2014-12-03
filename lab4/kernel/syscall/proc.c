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
#include <config.h>

#include <arm/reg.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/physmem.h>
#include <device.h>

#define SFROM_START 0x00000000
#define SFROM_END 0x00ffffff
#define SDRAM_START 0xa0000000
#define SDRAM_END 0xa3ffffff

/* Verifies that the buffer is entirely in valid memory. */
int check_mem(char *buf, int count, unsigned start, unsigned end) {
	unsigned start_buf = (unsigned) buf;
	unsigned end_buf = (unsigned)(buf + count);

	if ( (start_buf < start) || (start_buf > end) ) {
		return FALSE;
	}
	if ( (end_buf < start) || (end_buf > end) ) {
		return FALSE;
	}
	// Overflow case.
	if ( start_buf >= end_buf ) {
		return FALSE;
	}

	return TRUE;
}

// TODO: 
// ESCHED The given task set is not schedulable – some tasks may not meet their deadlines.

int task_create_syscall(task_t* tasks , size_t num_tasks)
{
	if (num_tasks > OS_AVAIL_TASKS) {
		return EINVAL;
	} else if (check_mem((char *) tasks, (int) num_tasks, SDRAM_START, SDRAM_END) == FALSE) {
		return EFAULT;
	}
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

int event_wait_syscall(unsigned int dev)
{
	if (dev >= NUM_DEVICES) {
		return EINVAL;
	}
	dev_wait(dev);
	// Add stuff to start next task
	dispatch_sleep();
	return 0;
}

/* An invalid syscall causes the kernel to exit. */
void invalid_syscall(unsigned int call_num  __attribute__((unused)))
{
	printf("Kernel panic: invalid syscall -- 0x%08x\n", call_num);

	disable_interrupts();
	while(1);
}
