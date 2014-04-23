#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void printmyaddress_ (char *str, void *var, int lenstr)
{
  char *locstr;

  locstr = malloc (lenstr+1);
  strncpy (locstr, str, lenstr);
  locstr[lenstr] = '\0';
  printf ("%s address=%p\n", locstr, var);
  free (locstr);
  return;
}
