/**
 * @file device.c
 *
 * @brief Implements simulated devices.
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-12-01
 */

#include <types.h>
#include <assert.h>
#include <exports.h>

#include <task.h>
#include <sched.h>
#include <device.h>
#include <arm/reg.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include <bits/errno.h>

#define NULL ((void *) 0)
/**
 * @brief Fake device maintainence structure.
 * Since our tasks are periodic, we can represent 
 * tasks with logical devices. 
 * These logical devices should be signalled periodically 
 * so that you can instantiate a new job every time period.
 * Devices are signaled by calling dev_update 
 * on every timer interrupt. In dev_update check if it is 
 * time to create a tasks new job. If so, make the task runnable.
 * There is a wait queue for every device which contains the tcbs of
 * all tasks waiting on the device event to occur.
 */

struct dev
{
	tcb_t* sleep_queue;
	unsigned long   next_match;
};
typedef struct dev dev_t;

/* devices will be periodically signaled at the following frequencies */
const unsigned long dev_freq[NUM_DEVICES] = {100, 200, 500, 50};
static dev_t devices[NUM_DEVICES];

/**
 * @brief Initialize the sleep queues and match values for all devices.
 */
void dev_init(void)
{
	int i;
	for(i = 0; i < NUM_DEVICES; i ++)
	{
		devices[i].sleep_queue = NULL;
		devices[i].next_match = dev_freq[i];
	}
}


/**
 * @brief Puts a task to sleep on the sleep queue until the next
 * event is signalled for the device.
 *
 * @param dev  Device number.
 */
int dev_wait(unsigned int dev)
{
	//printf("dev_wait hit %d \n", dev);
	tcb_t* currTcb = get_cur_tcb();
	if(currTcb->holds_lock == 1)
		return EHOLDSLOCK;
	dev_t* device = &(devices[dev]);
	tcb_t* prev = device->sleep_queue;
	device->sleep_queue = currTcb;
	currTcb->sleep_queue = prev;
	return 1;
	//if(device.sleep_queue == NULL)
		//printf("null. Lol tyou suck \n");
}


/**
 * @brief Signals the occurrence of an event on all applicable devices. 
 * This function should be called on timer interrupts to determine that 
 * the interrupt corresponds to the event frequency of a device. If the 
 * interrupt corresponded to the interrupt frequency of a device, this 
 * function should ensure that the task is made ready to run 
 */
void dev_update(unsigned long millis)
{
	//printf("within dev update \n");
	int i, j;
	j = 0; // flag if device ready
	for(i = 0; i < NUM_DEVICES; i++)
	{
		if(millis % dev_freq[i] == 0)
		{
			//printf("inside if statement, get device %d \n", i);
			dev_t* device = &(devices[i]);
			//printf("get task from sleep queue \n");
			tcb_t* task = device->sleep_queue;
			while(task != NULL)
			{
				j = 1;
				//printf("adding task to runqueue %d\n", i);
				runqueue_add(task, task->native_prio);
				//printf("2 \n");
				task = task->sleep_queue;
				//printf("3 \n");
			}
			//device->next_match += dev_freq[i];
			device->sleep_queue = NULL;
		}
	}

	if (j == 1) {
		//printf("now dispatch saving \n");
		dispatch_save();
	}
}

