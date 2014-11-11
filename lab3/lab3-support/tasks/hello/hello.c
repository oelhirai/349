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

int main(int argc, char** argv)
{
	const char hello[] = "Hello World\r\n";
	int volatile i = 0;
	int volatile temp;
	write(STDOUT_FILENO, hello, sizeof(hello) - 1);
	write(STDOUT_FILENO, hello, sizeof(hello) - 1);
	while(1){
		temp = 5;
		i = 7;
	}
	return 0;
}
