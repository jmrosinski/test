#include <stdio.h>

__device__ static int *arrptr;
__device__ static int x;

extern "C" {
__device__ void sub1_()
{
  arrptr = (int *) malloc (10);
  x = 11;
  printf ("sub1: arrptr=%p\n", arrptr);
  printf ("sub1: x=%d\n", x);
}


__device__ void sub2_()
{
  printf ("sub2: arrptr=%p\n", arrptr);
  printf ("sub2: x=%d\n", x);
}
}
