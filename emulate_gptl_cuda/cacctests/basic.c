#include <stdio.h>
#include "../src/gptl.h"
#include "../cuda/src/gptl_acc.h"

int main ()
{
  int n, ret;
  int zero = 0;

  ret = GPTLstart (0);
#pragma acc parallel private(ret) copyin(n)
  for (n = 0; n < 2; ++n) {
    ret = GPTLstart_gpu (n);
    ret = GPTLstop_gpu (n);
  }
#pragma end parallel
  ret = GPTLstop (0);
  ret = cudaDeviceSynchronize ();
  return 0;
}
