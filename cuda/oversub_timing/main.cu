#include <stdio.h>
#include <limits.h>
#include <cuda.h>
#include <gptl.h>
#include <gptl_cuda.h>

__global__ void set_baseclock (long long *);
__global__ void sub (long long *, long long *, long long *, int, int, float);
__device__ long long *baseclock;

// managed variables
static long long *minstamp;
static long long *maxstamp;

#define gpuErrchk(ans) { gpuAssert((ans), __FILE__, __LINE__); }
  inline void gpuAssert(cudaError_t code, const char *file, int line, bool abort=true)
  {
    if (code != cudaSuccess) 
      {
	fprintf(stderr,"GPUassert: %s %s %d\n", cudaGetErrorString(code), file, line);
	if (abort) exit(code);
      }
  }

int main ()
{
  const int smcount      = 5;
  const int warps_per_sm = 4;
  const int warpsize     = 32;
  int w, sm, i;
  int oversub;
  int nblocks;
  cudaError_t cudaret;
  int ret;
  float sleepsec;

  printf ("Enter oversubscription factor\n");
  scanf ("%d", &oversub);
  printf ("oversub=%d\n", oversub);

  printf ("Enter sleep time in floating point seconds\n");
  scanf ("%f", &sleepsec);
  printf ("sleepsec=%f\n", sleepsec);

  // Need GPTL lib for GPTLmy_sleep
  ret = GPTLinitialize ();

  gpuErrchk (cudaMalloc (&baseclock, smcount * sizeof (long long)));

  nblocks = oversub * smcount * warps_per_sm;
  gpuErrchk (cudaMallocManaged (&minstamp, nblocks * sizeof (long long)));
  gpuErrchk (cudaMallocManaged (&maxstamp, nblocks * sizeof (long long)));
  for (i = 0; i < nblocks; ++i) {
    minstamp[i] = LLONG_MAX;
    maxstamp[i] = 0;
  }
  cudaDeviceSynchronize ();

  // Set baseclock for each SM
  set_baseclock <<<smcount,warpsize*warps_per_sm>>> (baseclock);
  cudaret = cudaGetLastError();
  printf("%s\n", cudaGetErrorString(cudaret));
  cudaDeviceSynchronize ();
  cudaret = cudaGetLastError();
  printf("%s\n", cudaGetErrorString(cudaret));

  sub <<<nblocks,warpsize>>> (baseclock, minstamp, maxstamp, warps_per_sm, warpsize, sleepsec);
  cudaret = cudaGetLastError();
  printf("%s\n", cudaGetErrorString(cudaret));
  cudaDeviceSynchronize ();
  cudaret = cudaGetLastError();
  printf("%s\n", cudaGetErrorString(cudaret));

  cudaDeviceSynchronize ();

  for (sm = 0; sm < smcount; ++sm) {
    for (w = 0; w < warps_per_sm; ++w) {
      int idx = sm*warps_per_sm + w;
      if (maxstamp[idx] > 0) {
	printf ("w=%d sm=%d minstamp=%lld maxstamp=%lld\n", w, sm, minstamp[idx], maxstamp[idx]);
      }
    }
  }
}

__global__ void set_baseclock (long long *baseclock)
{
  uint smid;

  if (threadIdx.x > 0)
    return;
  if (blockDim.y != 1 || blockDim.z != 1) {
    printf ("blockDim.y and blockDim.z must all be 1\n");
    return;
  }

  asm volatile ("mov.u32 %0, %smid;" : "=r"(smid));
  baseclock[smid] = clock64();
}

__global__ void sub (long long *baseclock, long long *minstamp, long long *maxstamp,
		     int warps_per_sm, int warpsize, float sleepsec)
{
  uint smid;        // 0-smcount
  long long stamp;  // time stamp
  int idx;          // index into 2d array
  int w = (threadIdx.x + blockIdx.x*blockDim.x) / warpsize;  // linearized warp number

  if (threadIdx.x > 0)
    return;
  if (blockDim.y != 1 || blockDim.z != 1) {
    printf ("blockDim.y and blockDim.z must all be 1\n");
    return;
  }

  asm volatile ("mov.u32 %0, %smid;" : "=r"(smid));
  printf ("sub: smid=%d w=%d\n", smid, w);
  
  idx = smid*warps_per_sm + w;
  
  stamp = clock64() - baseclock[smid];
  if (stamp < minstamp[idx])
    minstamp[idx] = stamp;

  GPTLmy_sleep (sleepsec);

  stamp = clock64() - baseclock[smid];
  if (stamp > maxstamp[idx])
    maxstamp[idx] = stamp;

  __syncthreads ();
}
