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
	printf("no error \n");
	return temp; 
}

int mutex_lock(int m)
{	
	printf("we got to mutex lock yo %d \n", m);
	if(m > mut_cnt)
		return EINVAL;
	mutex_t* mutex = &gtMutex[m];
	tcb_t* currTCB = get_cur_tcb();
	if(mutex->pHolding_Tcb == currTCB )
		return EDEADLOCK;
	if(mutex->bAvailable == TRUE)
	{
		printf("the mutex is available \n");
		mutex->bAvailable = FALSE;
		mutex->pHolding_Tcb = currTCB;
		currTCB->holds_lock = 1;
		mutex->bLock = 1;
		// TODO : remove the next comment
		// Error check if we fuck up.
		mutex->pSleep_queue = (tcb_t *) NULL;
	}
	else
	{
		tcb_t* next = mutex->pSleep_queue;
		if(next == (tcb_t*) NULL)
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
		if(waiter->cur_prio < highest_prio())
			dispatch_save();
	}

	return 0; 
}

