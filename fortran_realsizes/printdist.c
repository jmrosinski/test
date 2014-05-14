#include <stdio.h>

void printdist_ (char *str, void *first, void *second)
{
  printf ("%s: distance is %d bytes\n", str, (int) (second - first));
}
