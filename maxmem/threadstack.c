#include <stdio.h>
#include <string.h>
#include <omp.h>

int main ()
{
  void sub (int);
  int iter;

  printf ("Testing thread stacksize of %d MB\n", 100);
#pragma parallel for
  for (iter=0; iter<256; iter++) {
    sub (iter);
  }
  printf ("Success\n");
  return 0;
}

void sub (int iter)
{
  int i;
  static const int megabyte = 1048576;
  struct {
    char x[megabyte];
  } bigthing[100];

  for (i=0; i<100; i++)
    memset (bigthing[i].x, 0, megabyte);
  return;
}
