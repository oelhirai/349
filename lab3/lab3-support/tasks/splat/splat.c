/** @file splat.c
 *
 * @brief Displays a spinning cursor.
 * Omer Elhiraika
 * Shashwat Srivastava
 * 
 */
#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv)
{

	int i;
	// instantiate characters for rotation
	char to_display[] = "|/-\\";
	while(1) {
		i = 0;
		for (i = 0; i < 4; i++) {
			// clear stdout and print rotaion characters in order
			printf("\r%c", to_display[i]);
			sleep(200);
		}
	}
}
