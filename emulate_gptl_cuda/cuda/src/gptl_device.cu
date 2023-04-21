#include <stdio.h>
#include "./gptl_cuda.h"

extern "C" {
  __device__ int GPTLstart_gpu (int handle)
  {
    printf ("GPTLstart_gpu: input handle=%d\n", handle);
    return 0;
  }

  __device__ int GPTLstop_gpu (int handle)
  {
    printf ("GPTLstop_gpu: input handle=%d\n", handle);
    return 0;
  }
}
