#include <omp.h>

void saxpy (float a, float *x, float *y, const int sz, const int nblocks)
{
  // target says run on GPU if possible
  // teams says says run in parallel
#pragma omp target teams map(to:x[0:sz]) map(tofrom:y[0:sz]) num_teams(nblocks)
  {
    int bs = sz / nblocks;
#pragma omp distribute
    for (int i = 0; i < sz; i += bs) {
#pragma omp parallel for simd firstprivate(i,bs)
      for (int ii = i; ii < i + bs; ++ii) {
	y[ii] = a*x[ii] + y[ii];
      }
    }
  }
}
