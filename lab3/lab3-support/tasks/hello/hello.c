/** @file hello.c
 *
 * @brief Prints out Hello world using the syscall interface.
 *
 * Links to libc.
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date   2008-10-29
 */
#include <unistd.h>

void convertToStr(unsigned int n, char* str, int count);
int getSize(int n);

int main(int argc, char** argv)
{
	const char hello[] = "Hello World\r\n";
	//int volatile i = 0;
	//int volatile temp;
	//int timeNow = 0;
	//int timeNew = 0;
	write(STDOUT_FILENO, hello, sizeof(hello) - 1);
	write(STDOUT_FILENO, hello, sizeof(hello) - 1);
	//printf("%d %d\n", timeNow, timeNew);
	uint32_t now = time();

	sleep(1000);
	uint32_t later = time();
	write(STDOUT_FILENO, hello, sizeof(hello) - 1);
	sleep(later - now);
	write(STDOUT_FILENO, hello, sizeof(hello) - 1);
	now = time();
	/*int size = getSize(now);
	char ans[size + 1];
	convertToStr(now, ans, size);
	write(STDOUT_FILENO, ans, sizeof(ans) - 1);*/
	return 0;
}

int getSize(int n)
{
	int count = 0;
	int n_copy = n;
	while(n_copy > 0)
	{
		count ++;
		n_copy /= 10;
	}
	return count;
}

void convertToStr(unsigned int n, char* ans, int count)
{
	
	//char ans[count + 1];
	int i = 0;
	for(i = 0; i < count; i ++)
	{
		*ans = 48 + (n %10);
		ans++;
		n /= 10;
	}
	ans[i] = '\0';
}

