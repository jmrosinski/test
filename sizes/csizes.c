#include <stdio.h>

void csizes_ ()
{
  printf ("pointer size=       %d\n", (int) sizeof (int *));
  printf ("float size=         %d\n", (int) sizeof (float));
  printf ("double size=        %d\n", (int) sizeof (double));
  printf ("long double size=   %d\n", (int) sizeof (long double));
  printf ("short size          %d\n", (int) sizeof (short));
  printf ("int size=           %d\n", (int) sizeof (int));
  printf ("long size=          %d\n", (int) sizeof (long));
  printf ("long long size=     %d\n", (int) sizeof (long long));
}
