#include <stdio.h>

__global__ void Kernel1 (int nz, int nip, int *d_kprox, float *d_arr)
{
  int ipn, ik, isn, k;

  ipn = blockIdx.x;
  k = threadIdx.x;

  ik = ipn*nz + k;
  d_arr[ik] = 0.;
  for (isn = 0; isn < d_kprox[k]; isn++) {
    d_arr[ik] += 1.;
  }
}

__host__ int main ()
{
  const int nz = 192;    // number of vertical points (threading)
  const int nip = 10242; // number of horizontal points (blocking)

  int k;                 // vertical index
  int ik;                // loop index collapses ipn and k
  int ipn;               // loop index over nip
  int isn;               // loop index over kprox
  int kprox[nz];         // mimics kprox in NIM
  int *d_kprox;          // kprox on device

  float refarr[nip*nz];  // reference array on CPU containing correct results
  float outarr[nip*nz];  // output array from GPU containing test results
  float *d_arr;

  dim3 cuda_threads1(nz);   // nz threads
  dim3 cuda_grids1(nip);    // nip blocks

  for (k = 0; k < nz; ++k) {
    kprox[k] = (k%6) + 1;
    for (ipn = 0; ipn < nip; ++ipn) {
      ik = ipn*nz + k;
      refarr[ik] = 0.;
      for (isn = 0; isn < kprox[k]; isn++) {
	refarr[ik] += 1.;
      }
    }
  }

  // Allocate space on device
  cudaMalloc ((void **) &d_arr,   nz*nip*sizeof(float));
  cudaMalloc ((void **) &d_kprox, nz*sizeof(int));

  // Set up kprox array for device
  cudaMemcpy (d_kprox, kprox, nz*sizeof(int), cudaMemcpyHostToDevice);

  Kernel1<<< cuda_grids1, cuda_threads1 >>>(nz, nip, d_kprox, d_arr);
  cudaThreadSynchronize ();

  // Copy results back to host
  cudaMemcpy (outarr, d_arr, nz*nip*sizeof(float), cudaMemcpyDeviceToHost);

  // Check results
  for (k = 0; k < nz; ++k) {
    for (ipn = 0; ipn < nip; ++ipn) {
      ik = ipn*nz + k;
      if (outarr[ik] != refarr[ik]) {
	printf ("ik=%d outarr=%f refarr=%f\n", ik, outarr[ik], refarr[ik]);
	return -1;
      }
    }
  }
  return 0;
}
