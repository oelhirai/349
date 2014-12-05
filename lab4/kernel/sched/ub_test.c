/** @file ub_test.c
 * 
 * @brief The UB Test for basic schedulability
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-11-20
 */

//#define DEBUG 0

#include <sched.h>
#ifdef DEBUG
#include <exports.h>
#endif

unsigned long hardcodedValues[64] ={1000 ,  828 ,  779 ,  756 ,  743 ,  734 ,  728 ,  724 ,  720 ,  717 ,  715 ,
							  713 ,  711 ,  710 ,  709 ,  708 ,  707 ,  706 ,  705 ,  705 ,  704 ,  704 , 
							   703 ,  703 ,  702 ,  702 ,  702 ,  701 ,  701 ,  701 ,  700 ,  700 ,  700 , 
							    700 ,  700 ,  699 ,  699 ,  699 ,  699 ,  699 ,  699 ,  698 ,  698 ,  698 ,
							      698 ,  698 ,  698 ,  698 ,  698 ,  697 ,  697 ,  697 ,  697 ,  697 ,  697 , 
							       697 ,  697 ,  697 ,  697 ,  697 ,  697 ,  697 ,  696 ,  696};
 
/**
 * @brief Perform UB Test and reorder the task list.
 *
 * The task list at the end of this method will be sorted in order is priority
 * -- from highest priority (lowest priority number) to lowest priority
 * (highest priority number).
 *
 * @param tasks  An array of task pointers containing the task set to schedule.
 * @param num_tasks  The number of tasks in the array.
 *
 * @return 0  The test failed.
 * @return 1  Test succeeded.  The tasks are now in order.
 */
int assign_schedule(task_t** tasks  , size_t num_tasks)
{
	task_t* sortedTasks[num_tasks];
	size_t i, j;
	for(i = 0; i < num_tasks; i ++)
	{
		sortedTasks[i] = (task_t *) &(tasks[i]);
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

	unsigned long accumalator = 0;
	for(i = 0; i < num_tasks; i++)
	{
		unsigned long T = sortedTasks[i]->T;
		unsigned long B = sortedTasks[i]->B;
		unsigned long C = sortedTasks[i]->C;
		unsigned long sum = accumalator + ((C + B) * 1000) / T;
		if(sum > hardcodedValues[i])
			return 0;
		accumalator += (C * 1000) / T;
	}
	return 1; 
}
	


