/**
 *
 * mycommand.c
 * Grundlagen der Programmierung 2
 * Testprogramm zu getopt()
 * Author: Alexander Nimmervoll
 * Last Modified: 2005-04-04
 */

#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <assert.h>

/* globale Variable fuer den Programmnamen */
char *program_name = NULL;

/* Funktion print_usage() zur Ausgabe der usage Meldung */
void print_usage()
{
   fprintf(stderr, "Usage: %s [-a] [-f filename] string1 [string2]\n", program_name);
   exit(EXIT_FAILURE);
}

/* main Funktion mit Argumentbehandlung */
int main(int argc, char *argv[])
{
   int c;
   int error = 0;
   char *inputFile = NULL;
   int cOptionA = 0;

   program_name = argv[0];

   while ((c = getopt(argc, argv, "af:")) != EOF)
   {
      switch (c)
      {
      case 'a':        /* Option ohne Argument */
         if (cOptionA) /* mehrmalige Verwendung? */
         {
            error = 1;
            break;
         }
         cOptionA = 1;
         printf("%s: parsing option a\n", program_name);
         break;
      case 'f':                 /* Option mit Argument */
         if (inputFile != NULL) /* mehrmalige Verwendung? */
         {
            error = 1;
            break;
         }
         inputFile = optarg; /* optarg zeigt auf Optionsargument */
         printf("%s: parsing option f, argument: %s\n", program_name, inputFile);
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
