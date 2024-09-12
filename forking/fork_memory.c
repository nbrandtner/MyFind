#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

int global_var = 1;

int main()
{
	pid_t pid;
	int local_var = 1;
	switch (pid = fork())
	{
	case -1:
		fprintf(stderr, "Fehler bei fork()\n");
		return EXIT_FAILURE;
	case 0:
		sleep(1); /* wait briefly */
		printf("--- In child process ---\n");
		printf("global_var = %d memory address : %p\n", global_var, &global_var);
		printf("local_var = %d memory address : %p\n", local_var, &local_var);
		fflush(stdout);
		++global_var;
		++local_var;
		printf("--- In child process ---\n");
		printf("global_var = %d memory address : %p\n", global_var, &global_var);
		printf("local_var = %d memory address : %p\n", local_var, &local_var);
		fflush(stdout);
		break;
	default:
		printf("--- In parent process ---\n");
		printf("global_var = %d memory address : %p\n", global_var, &global_var);
		printf("local_var = %d memory address : %p\n", local_var, &local_var);
		fflush(stdout);
		sleep(2);
		printf("--- In parent process ---\n");
		printf("global_var = %d memory address : %p\n", global_var, &global_var);
		printf("local_var = %d memory address : %p\n", local_var, &local_var);
		fflush(stdout);
		break;
	}
	return EXIT_SUCCESS;
}
