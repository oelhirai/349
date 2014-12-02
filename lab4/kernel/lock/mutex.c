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
#ifdef DEBUG_MUTEX
#include <exports.h> // temp
#endif

#define NULL ((char*)(0))

mutex_t gtMutex[OS_NUM_MUTEX];
int mut_cnt = -1;


void mutex_init()
{
	int i = 0;
	for(i = 0; i < OS_NUM_MUTEX; i++)
	{
		mutex_t* mutex = &gtMutex[i];
		mutex->bAvailable = TRUE;
		mutex->pHolding_Tcb = NULL;
		mutex->bLock = 0;
		mutex->pSleep_queue = NULL;
	}
}


int mutex_create(void)
{
	mut_cnt ++;
	if(mut_cnt > OS_NUM_MUTEX)
		return ENOMEM;
	return mut_cnt; 
}

int mutex_lock(int m)
{	
	if(m > mut_cnt)
		return EINVAL;
	mutex_t* mutex = &gtMutex[m];
	tcb_t* currTCB = get_cur_tcb();
	if(mutex->pHolding_Tcb == currTCB )
		return EDEADLOCK;
	if(mutex->bAvailable == TRUE)
	{
		mutex->bAvailable = FALSE;
		mutex->pHolding_Tcb = curr;
		currTCB->holds_lock = 1;
		mutex->bLock = 1;
		// TODO : remove the next comment
		// Error check if we fuck up.
		mutex->pSleep_queue = NULL;
	}
	else
	{
		tcb_t* next = mutex->pSleep_queue;
		if(next == NULL)
		{
			mutex->pSleep_queue = currTCB;
		}
		else
		{
			while(next->sleep_queue != NULL)
			{
				next = next->sleep_queue;
			}
			
			next->sleep_queue = currTCB;
		}
		currTCB->sleep_queue = NULL;
		dispatch_sleep();
	}
	return 0; 
}

int mutex_unlock(int m)
{
	if(m > mut_cnt)
		return EINVAL;

	tcb_t* currTCB = get_cur_tcb();
	mutex_t* mutex = &gtMutex[m];

	if(mutex->pHolding_Tcb != currTCB )
		return EPERM;

	currTCB->holds_lock = 0;

	if(mutex->pSleep_queue == NULL)
	{
		mutex->bAvailable = TRUE;
		mutex->pHolding_Tcb = NULL;
		mutex->bLock = 0;
	}
	else
	{
		tcb_t* waiter = mutex->pSleep_queue;
		mutex->pSleep_queue = waiter->sleep_queue;
		runqueue_add(waiter, waiter->cur_prio);
		mutex->pHolding_Tcb = waiter;
		waiter->holds_lock = 1;
		if(waiter->cur_prio < highest_prio)
			dispatch_save();
	}

	return 0; 
}

