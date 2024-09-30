/**
 *
 * myfind.c
 * myfind - ein einfacher Befehl, der Dateien in einem Verzeichnisbaum sucht
 * Author: Niklas Brandtner
 * Last Modified: 2024-09-26
 */

#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

/* globale Variable fuer den Programmnamen */
char *program_name = NULL;

/* Funktion print_usage() zur Ausgabe der usage Meldung */
void print_usage()
{
    fprintf(stderr, "Usage: %s [-R] [-i] searchpath filename1 [filename2] … [filenameN]\n", program_name);
    exit(EXIT_FAILURE);
}

/* main Funktion mit Argumentbehandlung */
int main(int argc, char *argv[])
{
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
            recursive = 1;
            printf("%s: parsing option R\n", program_name);
            break;
        case 'i':            /* Option mit Argument */
            if (insensitive) /* mehrmalige Verwendung? */
            {
                error = 1;
                break;
            }
            insensitive = 1;
            printf("%s: parsing option i\n", program_name);
            break;

        case '?': /* ungueltiges Argument */
            error = 1;
            break;
        default: /* unmoeglich */
            assert(0);
        }
    }
    if (error) /* Optionen fehlerhaft ? */
    {
        print_usage();
    }
    if ((argc < optind + 1) || (argc > optind + 2)) /* falsche Anzahl an Optionen */
    {
        print_usage();
    }

    /* Die restlichen Argumente, die keine Optionen sind, befinden sich in
     * argv[optind] bis argv[argc-1]
     */
    while (optind < argc)
    {
        /* aktuelles Argument: argv[optind] */
        printf("%s: parsing argument %s\n", program_name, argv[optind]);

        optind++;
    }

    return EXIT_SUCCESS;
}
