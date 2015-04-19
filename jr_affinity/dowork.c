#include <gptl.h>
#include <mpi.h>
#include <math.h>
#include <omp.h>
#include <stdio.h>

void dowork (const int niter, float results[niter])
{
  int iter;
  int ret;
  void compute (float *);

  ret = GPTLinitialize ();

#pragma omp parallel for private (ret)
  for (iter=0; iter<niter; ++iter) {
    ret = GPTLstart ("work");
    compute (&results[niter]);
    ret = GPTLstop ("work");
  }
  
  ret = GPTLpr_summary (MPI_COMM_WORLD);
  return;
}

void compute (float *result)
{
  int n;

  *result = 0.;
  for (n=0; n<1000000; ++n)
    *result += log(n+1);
}
