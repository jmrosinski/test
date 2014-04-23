#include <stdio.h>

void fsizes_ (void *f1, void *f2, char *string, int nc)
{
  int i;

  for (i = 0; i < nc; ++i)
    printf ("%c", string[i]);
  printf ("= %d\n", (int) (f2 - f1));
}
