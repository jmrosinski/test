#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

int error (const char *fmt, ...)
{
  va_list args;
  
  va_start (args, fmt);
  
  if (fmt != NULL)
    (void) vfprintf (stderr, fmt, args);
  exit (1);
}
