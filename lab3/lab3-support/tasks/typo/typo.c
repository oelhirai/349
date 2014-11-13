/** @file typo.c
 *
 * @brief Echos characters back with timing data.
 *
 * Links to libc.
 */
#include <stdio.h>
#include <unistd.h>
int main(int argc, char** argv)
{
 	int block_size = 128,;
 	char buf[block_size];
 	int read_result, write_result, i;
 	ssize_t len;  
 	unsigned long time_now, time_after;     

 	time_now = time();
 	read_result = read(STDIN_FILENO, buf, block_size);
 	while (read_result > 0) {             
 		time_after = time();             
		write_result = write(STDOUT_FILENO, buf, read_result);
		printf('')
		time_now = time();
		read_result = read(STDIN_FILENO, buf, block_size);      
	}                                                          

	if (read_result < 0) exit(1);                              
	exit(0);                                                   
	return 0;
}
