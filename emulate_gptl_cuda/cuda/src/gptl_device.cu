#include <stdio.h>
#include "./gptl_cuda.h"

extern "C" {
  __device__ int GTLstart_gpu (int handle)
  {
    printf ("GPTLstart_gpu: input handle=%d\n", handle);
    return 0;
  }

  __device__ int GTLstop_gpu (int handle)
  {
    printf ("GPTLstop_gpu: input handle=%d\n", handle);
    return 0;
  }
}
