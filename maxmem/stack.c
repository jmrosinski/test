#include <stdio.h>
#include <string.h>

int main ()
{
  void sub (void);

  printf ("Testing stacksize of %d MB\n", 100);
  sub ();
  printf ("Success\n");
  return 0;
}

void sub ()
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
