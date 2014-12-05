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

int[64] hardcodedValues = [ 1000 ,  828 ,  779 ,  756 ,  743 ,  734 ,  728 ,  724 ,  720 ,  717 ,  715 ,
							  713 ,  711 ,  710 ,  709 ,  708 ,  707 ,  706 ,  705 ,  705 ,  704 ,  704 , 
							   703 ,  703 ,  702 ,  702 ,  702 ,  701 ,  701 ,  701 ,  700 ,  700 ,  700 , 
							    700 ,  700 ,  699 ,  699 ,  699 ,  699 ,  699 ,  699 ,  698 ,  698 ,  698 ,
							      698 ,  698 ,  698 ,  698 ,  698 ,  697 ,  697 ,  697 ,  697 ,  697 ,  697 , 
							       697 ,  697 ,  697 ,  697 ,  697 ,  697 ,  697 ,  696 ,  696];
 
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
int assign_schedule(task_t** tasks  __attribute__((unused)), size_t num_tasks  __attribute__((unused)))
{

	return 1; // fix this; dummy return to prevent compiler warnings	
}
	


