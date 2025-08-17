#include <stdio.h>

extern "C" {
  __device__ int get_warp_num (void);
  
  __device__ int whoami (void)
  {
    int w;             // warp index (of this thread)
    uint smid;
    
    w = get_warp_num ();

    // Get the smid.
    
    asm volatile ("mov.u32 %0, %smid;" : "=r"(smid));
    if (w >= 0)
      printf ("Hello from warp=%d running on smid=%u\n", w, smid);
    return 0;
  }

  __device__ int get_warp_num ()
  {
    register int threadId;
    register int warpId;
    const int warpsize = 32;

    threadId = threadIdx.x
      +  blockDim.x  * threadIdx.y
      +  blockDim.x  *  blockDim.y  * threadIdx.z
      +  blockDim.x  *  blockDim.y  *  blockDim.z  * blockIdx.x
      +  blockDim.x  *  blockDim.y  *  blockDim.z  *  gridDim.x  * blockIdx.y
      +  blockDim.x  *  blockDim.y  *  blockDim.z  *  gridDim.x  *  gridDim.y  * blockIdx.z;

    if (threadId % warpsize == 0)
      warpId = threadId / warpsize;
    else
      warpId = -1;
    
    return warpId;
  }
}
