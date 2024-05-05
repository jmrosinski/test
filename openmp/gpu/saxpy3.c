#include <omp.h>

void saxpy3 (float a, float *x, float *y, const int sz, const int nblocks)
{
#pragma omp target teams distribute parallel for simd	\
  num_teams(nblocks) map(to:x[0:sz]) map(tofrom:y[0:sz])
    for (int i = 0; i < sz; ++i) {
      y[i] = a*x[i] + y[i];
    }
}
