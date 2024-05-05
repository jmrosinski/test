#include <omp.h>

void saxpy2 (float a, float *x, float *y, const int sz, const int nblocks)
{
  // target says run on GPU if possible
  // teams defines top level parallelism
  // distribute says iterations of 1 or more loops will be executed by thread teams
  // simd says the instructions in the loop are simd
#pragma omp target teams distribute parallel for simd num_teams(nblocks) \
  map(to:x[0:sz]) map(tofrom:y[0:sz])
  for (int i = 0; i < sz; ++i) {
    y[i] = a*x[i] + y[i];
  }
}
