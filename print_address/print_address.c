#include <stdio.h>

void print_address_ (int *n, void *var, char*str)
{
  printf ("n=%d str=%s addr=%p\n", *n, str, var);
}
