/**
 *
 * myfind.c
 * myfind - ein einfacher Befehl, der Dateien in einem Verzeichnisbaum sucht
 * Author: Niklas Brandtner
 * Last Modified: 2024-09-26
 */
#define _DEFAULT_SOURCE
#include <sys/file.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <limits.h>
#include <ctype.h>
#ifndef PATH_MAX
#define PATH_MAX 255
#endif

volatile sig_atomic_t children_working = 0; // Counter to track the number of children finished

/* globale Variable fuer den Programmnamen */
char *program_name = NULL;

// Signal handler for SIGUSR1
void handle_sigusr1(int sig)
{
    // printf("Parent: Received SIGUSR1, a child finished its work.\n");
    children_working--;
}

/* Funktion print_usage() zur Ausgabe der usage Meldung */
void print_usage()
{
    fprintf(stderr, "Usage: %s [-R] [-i] searchpath filename1 [filename2] … [filenameN]\n", program_name);
    exit(EXIT_FAILURE);
}

void synchronized_print(const char *message)
{
    int fd = fileno(stdout); // Get the file descriptor for stdout
    flock(fd, LOCK_EX);      // Acquire an exclusive lock

    // Print the message
    printf("%s", message);
    fflush(stdout); // Ensure the output is flushed

    flock(fd, LOCK_UN); // Release the lock
}

void searchForFile(char *filename, char *searchpath, bool i)
{
    struct dirent *direntp;
    DIR *dirp;

    // Open the directory specified by searchpath
    if ((dirp = opendir(searchpath)) == NULL)
    {
        perror("Failed to open directory");
        return;
    }

    // Convert filename to lowercase if case-insensitive search is enabled
    if (i)
    {
        for (char *p = filename; *p; ++p)
        {
            *p = tolower(*p);
        }
    }

    // Read each entry in the directory
    while ((direntp = readdir(dirp)) != NULL)
    {
        // Skip hidden files and directories
        if (direntp->d_name[0] == '.')
        {
            continue;
        }

        char f_name[PATH_MAX];
        strncpy(f_name, direntp->d_name, PATH_MAX);

        // Convert directory entry name to lowercase if case-insensitive search is enabled
        if (i)
        {
            for (char *p = f_name; *p; ++p)
            {
                *p = tolower(*p);
            }
        }

        // Construct the full path of the directory entry
        char full_path[PATH_MAX];
        snprintf(full_path, PATH_MAX, "%s/%s", searchpath, direntp->d_name);

        // Get the absolute path of the directory entry
        char abs_path[PATH_MAX];
        if (realpath(full_path, abs_path) == NULL)
        {
            perror("Failed to get absolute path");
            continue;
        }

        // If the filename matches the directory entry name, print the result
        if (strcmp(filename, f_name) == 0)
        {
            char message[PATH_MAX + 100];
            snprintf(message, sizeof(message), "%d: %s: %s\n", getpid(), filename, abs_path);
            synchronized_print(message);
        }
    }
    closedir(dirp); // Close the directory
}

void searchForFileRecursive(char *filename, char *searchpath, bool i)
{
    struct dirent *direntp;
    DIR *dirp;

    // Open the directory specified by searchpath
    if ((dirp = opendir(searchpath)) == NULL)
    {
        perror("Failed to open directory");
        return;
    }

    // Convert filename to lowercase if case-insensitive search is enabled
    if (i)
    {
        for (char *p = filename; *p; ++p)
        {
            *p = tolower(*p);
        }
    }

    // Read each entry in the directory
    while ((direntp = readdir(dirp)) != NULL)
    {
        // Skip hidden files and directories
        if (direntp->d_name[0] == '.')
        {
            continue;
        }

        char f_name[PATH_MAX];
        strncpy(f_name, direntp->d_name, PATH_MAX);

        // Convert directory entry name to lowercase if case-insensitive search is enabled
        if (i)
        {
            for (char *p = f_name; *p; ++p)
            {
                *p = tolower(*p);
            }
        }

        // Construct the full path of the directory entry
        char full_path[PATH_MAX];
        snprintf(full_path, PATH_MAX, "%s/%s", searchpath, direntp->d_name);

        // Get the absolute path of the directory entry
        char abs_path[PATH_MAX];
        if (realpath(full_path, abs_path) == NULL)
        {
            perror("Failed to get absolute path");
            continue;
        }

        // If the filename matches the directory entry name, print the result
        if (strcmp(filename, f_name) == 0)
        {
            char message[PATH_MAX + 100];
            snprintf(message, sizeof(message), "%d: %s: %s\n", getpid(), filename, abs_path);
            synchronized_print(message);
        }

        // If the directory entry is a directory, recursively search it
        if (direntp->d_type == DT_DIR)
        {
            searchForFileRecursive(filename, full_path, i);
        }
    }
    closedir(dirp); // Close the directory
}

/* main Funktion mit Argumentbehandlung */

/* Für jeden gesuchten Filenamen wird einmal fork() aufgerufen und die child-pid in pids[] gespeichert.
Dann sucht das child rekursiv oder auch nicht nach dem filenamen und gibt den im geforderten Format aus.
Durch flock haben wir sichergestellt, dass niemals mehrere childprozesse gleichzeitig schreiben können. */
int main(int argc, char *argv[])
{
    struct sigaction sa;
    sa.sa_handler = handle_sigusr1;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGUSR1, &sa, NULL);

    int c;
    int error = 0;
    bool recursive = false;
    bool insensitive = false;

    program_name = argv[0];

    while ((c = getopt(argc, argv, "Ri")) != EOF)
    {
        switch (c)
        {
        case 'R':          /* Option ohne Argument */
            if (recursive) /* mehrmalige Verwendung? */
            {
                error = 1;
                break;
            }
            recursive = true;
            printf("%s: parsing option R\n", program_name);
            break;
        case 'i':            /* Option mit Argument */
            if (insensitive) /* mehrmalige Verwendung? */
            {
                error = 1;
                break;
            }
            insensitive = true;
            printf("%s: parsing option i\n", program_name);
            break;

        case '?': /* ungueltiges Argument */
            error = 1;
            break;
        default: /* unmoeglich */
            assert(0);
        }
    }
    if (error || (argc <= optind)) /* Invalid number of arguments */
    {
        printf("anzahl an optionen fehlerhaft\n");
        print_usage();
    }

    /* Die restlichen Argumente, die keine Optionen sind, befinden sich in
     * argv[optind] bis argv[argc-1]
     */
    char *searchpath = argv[optind];
    optind++;

    /* create array */
    int cnt = argc - 2;
    if (recursive == true)
    {
        cnt--;
    }
    if (insensitive == true)
    {
        cnt--;
    }
    int num_files = argc - optind;                      // Number of files to search for
    int *pids = (int *)malloc(num_files * sizeof(int)); // Store child PIDs

    for (int i = 0; i < num_files; i++)
    {
        char *filename = argv[optind + i];
        pids[i] = fork();
        if (pids[i] < 0)
        {
            perror("fork fail");
            exit(1);
        }
        if (pids[i] == 0)
        {
            // Child process
            if (!recursive)
            {
                printf("Child %d was created and is searching for '%s'\n", getpid(), filename);
                searchForFile(filename, searchpath, insensitive);
            }
            else
            {
                printf("Child %d was created and is searching for '%s'\n", getpid(), filename);
                searchForFileRecursive(filename, searchpath, insensitive);
            }
            // Signal the parent that this child has finished
            printf("Child %d: Sending SIGUSR1 to parent.\n", getpid()); // Debugging output
            kill(getppid(), SIGUSR1);
            exit(0); // Child kills itself
        }
        else
        {
            // Parent process
            children_working++;
        }
    }

    // Wait for all child processes to finish
    for (int i = 0; i < num_files; i++)
    {
        int status;
        waitpid(pids[i], &status, 0);
        if (WIFEXITED(status))
        {
            // printf("Parent: Child %d exited with status %d.\n", pids[i], WEXITSTATUS(status));
        }
        else if (WIFSIGNALED(status))
        {
            // printf("Parent: Child %d was terminated by signal %d.\n", pids[i], WTERMSIG(status));
        }
    }

    free(pids);
    printf("Parent: All children finished.\n");

    return EXIT_SUCCESS;
}
