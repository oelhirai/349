/** @file typo.c
 *
 * @brief Echos characters back with timing data.
 *
 * Links to libc.
 */
#include <stdio.h>
#include <unistd.h>
#include <bits/swi.h>

int main(int argc, char** argv)
{
 	int block_size = 128;
 	char buf[block_size];
 	int read_result, write_result;
 	unsigned long time_now, time_after; 
 	unsigned int the_time;    

 	time_now = time();
 	read_result = read(STDIN_FILENO, buf, block_size);
 	while (read_result > 0) {             
 		time_after = time();             
		write_result = write(STDOUT_FILENO, buf, read_result);
		the_time = (time_after - time_now);
		int frac = (the_time % 1000) / 100;
		int nonFrac = (the_time / 1000);
		the_time /= 1000;
		printf("%d.%ds\n", nonFrac, frac);
		time_now = time();
		read_result = read(STDIN_FILENO, buf, block_size);      
	}                                                          

	if (read_result < 0) return 1;                              
	return 0;
}
