/** @file splat.c
 *
 * @brief Displays a spinning cursor.
 *
 * Links to libc.
 */
#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv)
{
	/* Add code here -- put your names at the top. */
	int i;
	char to_display[] = "|/-\\";
	while(1) {
		i = 0;
		for (i = 0; i < 4; i++) {
			printf("\r%c", to_display[i]);
			sleep(200);
		}
	}
}
