/** @file dagger.c
 *
 * @brief Creates two simple periodic tasks.
 *
 * @note  This is like knife -- but smaller :)
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date   2008-11-30
 */
#include <stdio.h>
#include <task.h>
#include <unistd.h>

int globes = 0;
int mutex = 0;

void panic(const char* str)
{
	puts(str);
	while(1);
}

void fun1(void* str)
{
	while(1)
	{
		putchar((int)str);
		mutex_lock(mutex);
		globes = 1;
		
		sleep(1000);
		printf("%d \n", globes);
		mutex_unlock(mutex);
		if (event_wait(0) < 0)
			panic("Dev 0 failed");
	}
}

void fun2(void* str)
{
	while(1)
	{
		putchar((int)str);
		mutex_lock(mutex);
		globes = 2;
		printf("%d \n", globes);
		mutex_unlock(mutex);
		if (event_wait(1) < 0)
			panic("Dev 1 failed");
	}
}

void fun3(void* str)
{
	while(1)
	{
		putchar((int)str);
		mutex_lock(mutex);
		globes = 3;
		printf("%d \n", globes);
		mutex_unlock(mutex);
		if (event_wait(1) < 0)
			panic("Dev 1 failed");
	}
}

void fun4(void* str)
{
	while(1)
	{
		putchar((int)str);
		mutex_lock(mutex);
		globes = 4;
		printf("%d \n", globes);
		mutex_unlock(mutex);
		if (event_wait(1) < 0)
			panic("Dev 1 failed");
	}
}

void fun5(void* str)
{
	while(1)
	{
		putchar((int)str);
		mutex_lock(mutex);
		globes = 5;
		printf("%d \n", globes);
		mutex_unlock(mutex);
		if (event_wait(1) < 0)
			panic("Dev 1 failed");
	}
}

int main(int argc, char** argv)
{
	task_t tasks[5];
	tasks[0].lambda = fun1;
	tasks[0].data = (void*)'@';
	tasks[0].stack_pos = (void*)0xa2000000;
	tasks[0].C = 1;
	tasks[0].T = PERIOD_DEV0;

	tasks[1].lambda = fun2;
	tasks[1].data = (void*)'<';
	tasks[1].stack_pos = (void*)0xa1000000;
	tasks[1].C = 1;
	tasks[1].T = PERIOD_DEV3;

	tasks[2].lambda = fun3;
	tasks[2].data = (void*)'<';
	tasks[2].stack_pos = (void*)0xa1001000;
	tasks[2].C = 1;
	tasks[2].T = PERIOD_DEV3;

	tasks[3].lambda = fun4;
	tasks[3].data = (void*)'<';
	tasks[3].stack_pos = (void*)0xa1002000;
	tasks[3].C = 1;
	tasks[3].T = PERIOD_DEV3;

	tasks[4].lambda = fun5;
	tasks[4].data = (void*)'<';
	tasks[4].stack_pos = (void*)0xa1003000;
	tasks[4].C = 1;
	tasks[4].T = PERIOD_DEV3;

	mutex = (int) mutex_create();
	task_create(tasks, 5);

	argc=argc; /* remove compiler warning */
	argv[0]=argv[0]; /* remove compiler warning */

	puts("Why did your code get here!\n");
	return 0;
}
