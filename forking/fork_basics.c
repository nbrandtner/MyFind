#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

int main()
{
   pid_t pid, wpid;
   int status;

   pid = fork();
   switch (pid)
   {
   case 1: /* error */
      fprintf(stderr, "myfork: error when forking child process\n");
      return EXIT_FAILURE;
   case 0: /* child process */
      printf("--- Child process with PID %d\n", (int)getpid());
      return 3;
   default: /* parent */
      printf("Parent process with PID %d\n", (int)getpid());
      while ((wpid = wait(&status)) != pid)
      {
         if (wpid != -1)
            continue; /* different child process has terminated, continue waiting */

         /* error waiting */
         fprintf(stderr, "myfork: error when waiting for child process\n");
         return EXIT_FAILURE;
      }
      /* check exit code of child after finishing */
      if (WIFEXITED(status)) /* child has finished normally with exit code WEXITSTATUS(status) */
      {
         printf("Child has finished normally, exit code: %d\n", WEXITSTATUS(status));
      }
      else /* child has finished with error */
      {
         printf("Child process has finished with error or via signal\n");
      }
   }
   return 0;
}
