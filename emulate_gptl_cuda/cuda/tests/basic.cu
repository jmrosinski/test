#include <stdio.h>
#include <cuda.h>
#include "../src/gptl_cuda.h"

__global__ void runit (void);

int main ()
{
  int ret;
  int nblocks   = 2;
  int blocksize = 3;

  runit<<<nblocks,blocksize>>> ();
  cudaDeviceSynchronize ();
  return 0;
}

__global__ void runit (void)
{
  int ret;
  
  printf ("Entered global routine runit\n");
  ret = GPTLstart_gpu (0);
  ret = GPTLstop_gpu (1);
}
