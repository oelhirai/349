/** @file echo.c
 *
 * @author  Harry Q Bovik (Change this!)
 * @date    The current date
 */

#include <unistd.h>
#include <bits/fileno.h>

#define MAX_BUFFER 1024

/* String length excluding null character. */
int string_length(char* yo) {
    int i = 0;
    while (i < MAX_BUFFER && yo[i] != 0) {
        i++;
    }
    return i;
}

int main(int argc, char** argv) {
    /* Put your code here */

    int i = 0;
    //printf("argc = %d\n", argc);

    for (i = 0; i < argc; i++) {
        //printf("argv[%d] = %s\n", i, argv[i]);

        int len = string_length(argv[i]);
        write(STDOUT_FILENO, argv[i], len);
        write(STDOUT_FILENO, "\n", 1);
    }

    return 0;
}
