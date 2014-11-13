/** @file hello.c
 *
 * @brief Prints out Hello world using the syscall interface.
 *
 * Links to libc.
 *
 * Omer Elhiraika
 * Shashwat Srivastava
 */
#include <unistd.h>
#include <stdio.h>

int main(int argc, char** argv)
{
	const char hello[] = "Hello World\r\n";

	write(STDOUT_FILENO, hello, sizeof(hello) - 1);
	write(STDOUT_FILENO, hello, sizeof(hello) - 1);
	printf("%d %d\n", 7, 8);
	uint32_t now = time();
	//printf("below time");

	sleep(1000);
	uint32_t later = time();
	write(STDOUT_FILENO, hello, sizeof(hello) - 1);
	sleep(later - now);
	write(STDOUT_FILENO, hello, sizeof(hello) - 1);
	now = time();

	return 0;
}

