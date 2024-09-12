#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    if (fork() == 0)
    {
        printf("Child: Hello from child!\n");
    }
    else
    {
        printf("Parent: Hello from parent!\n");
        wait(NULL);

        printf("Status: Child is finished\n");
    }

    sleep(4);

    printf("Bye!\n");

    return 0;
}
