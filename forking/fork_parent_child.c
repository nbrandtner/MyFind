#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void parent()
{
	pid_t pid = 0;
	int status = 0;
	pid = getpid();
	printf("Parent with PID: %d\r\n", pid);

	// wait for the child
	pid = wait(&status);
	printf("Child with pid: %d exited with Status: %d\r\n", pid, WEXITSTATUS(status));
}

void child(int argc, char **argv)
{
	pid_t pid;
	const char *hallo = "echo";
	pid = getpid();
	printf("Child with PID: %d\r\n", pid);

	execvp(hallo, argv);
}

int main(int argc, char **argv)
{
	pid_t pid;

	// split processes
	pid = fork();

	switch (pid)
	{
	case -1:
		printf("Child konnte nicht gestartet werden.");
		exit(EXIT_FAILURE);
		break;
	case 0:
		child(argc, argv);
		break;
	default:
		printf("Child with PID: %d created\r\n", pid);
		parent();

		break;
	}
	exit(EXIT_SUCCESS);
}
