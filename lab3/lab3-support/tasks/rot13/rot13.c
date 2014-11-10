/*
 * rot13.c: ROT13 cipher test application
 *
 * Authors: Omer Elhiraika <oelhirai@gmail.com>
 * Date:    12:05, 10/11/2014
 */

#include <stdlib.h>
#include <unistd.h>
#include <bits/fileno.h>
#include <bits/swi.h>

size_t strlen(const char *buf) {
	int len = 0;
	while (buf[len] != '\0') {
		len++;
	}
	return len;
}

int main(int argc, char *argv[]) {
 	int block_size = 128, shift = 13;
 	char buf[block_size];
 	char space[1];						
 	char a = 'a', z = 'z', cap_a = 'A', cap_z = 'Z';
 	int read_result, write_result, i;
 	ssize_t len;       

 	/*
 	 * print out parameters to the screen before cipher
 	 */
 	space[0] = (char) 10;			//instantiate buffer to hold new line
 	for (i = 0; i < argc; i++) {
 		len = strlen(argv[i]);
 		write(STDOUT_FILENO, (void *)argv[i], len);	// print argument
 		write(STDOUT_FILENO, (void *)&space[0], 1);	// and newline after
 	}

 	read_result = read(STDIN_FILENO, buf, block_size);
 	while (read_result > 0) {

		/* Basis of Algorithm
  		*
  		* For 'A' < Letter < 'Z' (and correspondingly, 'a' < Letter < 'z')
  		* shifted letter = ascii(A) + (shift + (letter - ascii(A))) % numLetters
  		* 
  		* With this method, if A < letter < M, then the shift is simply added to
  		* the offset of the current letter from A. if > M, then modding by the 
		* amount of letters results in effectively a -13 shift from current letter.
		*/       

		for (i = 0; i < read_result; i++) {
			if ((buf[i] >= cap_a) && (buf[i] <= cap_z)) {           
				buf[i] = cap_a + ((shift + buf[i] - cap_a) % 26);
			} else if (a <= buf[i] && buf[i] <= z) {                
				buf[i] = a + ((shift + buf[i] - a) % 26);       
			}                                     

		}                              
		write_result = write(STDOUT_FILENO, buf, read_result);
		if (write_result < 0) exit(1);            
		read_result = read(STDIN_FILENO, buf, block_size);      
	}                                                          

	if (read_result < 0) exit(1);                              
	exit(0);                                                   
	return 0;                                                  
}  
