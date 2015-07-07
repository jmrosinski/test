#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main ()
{
  const int mb = 1024*1024;
  long nbytes;
  char *arr;

  for (nbytes = 1000*mb; ;nbytes *= 1.1) {
    printf ("malloc asking for %f GB...\n", nbytes/(1024.*mb));
    if (arr = (char *) malloc (nbytes)) {
      (void) printf ("malloc succeeded, now memset...\n");
      (void) fflush (stdout);
      memset (arr, 0, nbytes);
      printf ("Success\n");
      (void) fflush (stdout);
      free (arr);
    } else {
      printf ("malloc failed. quitting.\n");
      exit (0);
    }
  }
}

