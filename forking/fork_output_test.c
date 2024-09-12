/* see: https://unix.stackexchange.com/questions/447898/why-does-a-program-with-fork-sometimes-print-its-output-multiple-times/447904 */

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    /* try also without \n or printing to stderr instead of stdout */
    /* which differences can you see in the output? what is the reason? */
    printf("hello world...\n");
    fork();
    fork();
    fork();
}