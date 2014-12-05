/**
 * @file mutex.c
 *
 * @brief Implements mutices.
 *
 * @author Harry Q Bovik < PUT YOUR NAMES HERE
 *
 * 
 * @date  
 */

//#define DEBUG_MUTEX

#include <lock.h>
#include <task.h>
#include <sched.h>
#include <bits/errno.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include <exports.h>

#ifdef DEBUG_MUTEX
#include <exports.h> // temp
#endif

#define NULL ((void*)(0))

mutex_t gtMutex[OS_NUM_MUTEX];
int mut_cnt = 0;


void mutex_init()
{
	int i = 0;
	for(i = 0; i < OS_NUM_MUTEX; i++)
	{
		mutex_t* mutex = &gtMutex[i];
		mutex->bAvailable = TRUE;
		mutex->pHolding_Tcb = (tcb_t*) NULL;
		mutex->bLock = 0;
		mutex->pSleep_queue = (tcb_t*) NULL;
	}
}


int mutex_create(void)
{
	int temp = mut_cnt;
	mut_cnt++;
	if(temp > OS_NUM_MUTEX)
		return ENOMEM;
	return temp; 
}

int mutex_lock(int m)
{	
	//check if locking previously unallocated
	if(m > mut_cnt || m < 0)
		return EINVAL;
	mutex_t* mutex = &gtMutex[m];
	tcb_t* currTCB = get_cur_tcb();

	// ensure this task is not holding on to its own lock
	if(mutex->pHolding_Tcb == currTCB )
		return EDEADLOCK;

	if(mutex->bAvailable == TRUE)
	{
		mutex->bAvailable = FALSE;
		mutex->pHolding_Tcb = currTCB;
		currTCB->holds_lock = 1;
		mutex->bLock = 1;
		mutex->pSleep_queue = (tcb_t *) NULL;
	}

	// lock is not free, the current task should sleep then 
	// switch to highest priority task
	else
	{
		tcb_t* holdingTcb = mutex->pHolding_Tcb;

		// set holding TCB as highest priority task (0th TCB)
		holdingTcb->cur_prio = 0;
		runqueue_add(holdingTcb, 0);
		tcb_t** system_tcb = get_system_tcb();
		system_tcb[0] = holdingTcb;
		dispatch_save();

		mutex->pHolding_Tcb = currTCB;
		holdingTcb->cur_prio = holdingTcb->native_prio;
		currTCB->holds_lock = 1;
		/*tcb_t* next = mutex->pSleep_queue;
		if(next == (tcb_t*) NULL)
		{
			// first task to enter sleep queue
			mutex->pSleep_queue = currTCB;
		}
		else
		{
			// not the first task, place task at end of sleep queue
			while(next->sleep_queue != NULL)
			{
				next = next->sleep_queue;
			}
			next->sleep_queue = currTCB;
		}
		currTCB->sleep_queue = NULL;
		dispatch_sleep();*/
	}
	return 0; 
}

int mutex_unlock(int m)
{
	//check if unlocking mutex previously unallocated
	if(m > mut_cnt || m < 0)
		return EINVAL;

	tcb_t* currTCB = get_cur_tcb();
	mutex_t* mutex = &gtMutex[m];

	// The current task does not hold the mutex
	if(mutex->pHolding_Tcb != currTCB )
		return EPERM;

	currTCB->holds_lock = 0;

	// restore to native priority
	currTCB->cur_prio = currTCB->native_prio;

	if(mutex->pSleep_queue == NULL)
	{
		// make this mutex available (last task to need mutex)
		mutex->bAvailable = TRUE;
		mutex->pHolding_Tcb = NULL;
		mutex->bLock = 0;
	}
	else
	{
		// remove this task from the mutex, wake task
		// waiting on this mutex, and run if it is of higher priority
		tcb_t* waiter = mutex->pSleep_queue;
		mutex->pSleep_queue = waiter->sleep_queue;
		runqueue_add(waiter, waiter->cur_prio);
		mutex->pHolding_Tcb = waiter;
		waiter->holds_lock = 1;
		if(waiter->cur_prio < highest_prio())
			dispatch_save();
	}

	return 0; 
}

