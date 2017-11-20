#include <stdio.h>
extern void sub1_ (void);

void ccode_ ()
{
#pragma acc routine (sub1_) seq
  printf ("sub1 on CPU invoking CUDA routine sub1\n");
#pragma acc kernels
  {
    sub1_ ();
  }
}
