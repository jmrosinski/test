#include <stdio.h>

__global__ void Kernel1 (int nz, int nip, int *d_nprox, float *d_arr)
{
  int ipn, ik, isn, k;

  ipn = blockIdx.x;
  k = threadIdx.x;

  //  if (k == 0)
  //    printf ("Kernel1: nprox[%d]=%d\n", ipn, d_nprox[ipn]);

  ik = ipn*nz + k;
  d_arr[ik] = 0.;
  for (isn = 0; isn < d_nprox[ipn]; isn++) {
    d_arr[ik] += 1.;
  }
  //  printf ("Kernel1: k=%d ik=%d d_arr=%f\n", k, ik, d_arr[ik]);
}

__host__ int main ()
{
  const int nz = 192;    // number of vertical points (threading)
  const int nip = 10242; // number of horizontal points (blocking)

  int k;                 // vertical index
  int ik;                // loop index collapses ipn and k
  int ipn;               // loop index over nip
  int isn;               // loop index over nprox
  int nprox[nip];        // mimics nprox in NIM
  int *d_nprox;          // nprox on device

  float refarr[nip*nz];  // reference array on CPU containing correct results
  float outarr[nip*nz];  // output array from GPU containing test results
  float *d_arr;

  dim3 cuda_threads1(nz);   // nz threads
  dim3 cuda_grids1(nip);    // nip blocks

  for (ipn = 0; ipn < nip; ++ipn) {
    nprox[ipn] = (ipn%6) + 1;
    for (k = 0; k < nz; ++k) {
      ik = ipn*nz + k;
      refarr[ik] = 0.;
      for (isn = 0; isn < nprox[ipn]; isn++) {
	refarr[ik] += 1.;
      }
    }
  }

  // Allocate space on device
  cudaMalloc ((void **) &d_arr,   nz*nip*sizeof(float));
  cudaMalloc ((void **) &d_nprox, nip*sizeof(int));

  // Set up nprox array for device
  cudaMemcpy (d_nprox, nprox, nip*sizeof(int), cudaMemcpyHostToDevice);

  Kernel1<<< cuda_grids1, cuda_threads1 >>>(nz, nip, d_nprox, d_arr);
  cudaThreadSynchronize ();

  // Copy results back to host
  cudaMemcpy (outarr, d_arr, nz*nip*sizeof(float), cudaMemcpyDeviceToHost);

  // Check results
  for (ipn = 0; ipn < nip; ++ipn) {
    for (k = 0; k < nz; ++k) {
      ik = ipn*nz + k;
      if (outarr[ik] != refarr[ik]) {
	printf ("ik=%d outarr=%f refarr=%f\n", ik, outarr[ik], refarr[ik]);
	return -1;
      }
    }
  }
  return 0;
}
