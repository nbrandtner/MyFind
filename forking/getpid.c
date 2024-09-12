#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    pid_t pid, ppid;

    pid = getpid();
    ppid = getppid();

    printf("The parent process ID is %d\n", (int)ppid);
    printf("The process process ID is %d\n", (int)pid);

    return 0;
}
