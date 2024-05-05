#include <stdio.h>
#include <omp.h>
#include "gptl.h"

extern void saxpy (float, float *, float *, const int, const int);
extern void saxpy2 (float, float *, float *, const int, const int);
extern void saxpy3 (float, float *, float *, const int, const int);
extern void saxpy_1copy (float, const int, float [], float [], const int);
extern void initialize (float *, float *, const int);
extern void print_results (float *, const int sz, char *);

#define MIN (A, B) ((A) < (B) ? (A) : (B))

int main()
{
  const int SZ = 100000;
  const int smcount = 5; // Number of SMs
  const int niter = 100;
  float x[SZ];
  float y[SZ];
  float a = 2.;
  double tb, te;         // timing
  int ret;
  
  //  printf ("_OPENMP is %d\n", _OPENMP);
  ret = GPTLsetutr (GPTLnanotime);
  ret = GPTLinitialize ();

  initialize (x, y, SZ);

  ret = GPTLstart ("saxpy");
  for (int n = 0; n < niter; ++n) {
    saxpy (a, x, y, SZ, smcount);
  }
  ret = GPTLstop ("saxpy");
  print_results (y, SZ, "saxpy");

  initialize (x, y, SZ);
  ret = GPTLstart ("saxpy2");
  for (int n = 0; n < niter; ++n) {
    saxpy2 (a, x, y, SZ, smcount);
  }
  ret = GPTLstop ("saxpy2");
  print_results (y, SZ, "saxpy2");

  initialize (x, y, SZ);
  ret = GPTLstart ("saxpy3");
  for (int n = 0; n < niter; ++n) {
    saxpy3 (a, x, y, SZ, smcount);
  }
  ret = GPTLstop ("saxpy3");
  print_results (y, SZ, "saxpy3");

  initialize (x, y, SZ);
  ret = GPTLstart ("saxpy_1copy");
  for (int n = 0; n < niter; ++n) {
    saxpy_1copy (SZ, a, x, y, smcount);
  }
  ret = GPTLstop ("saxpy_1copy");
  print_results (y, SZ, "saxpy_1copy");

  ret = GPTLpr (0);
}

void initialize (float *x, float *y, const int sz)
{
  for (int i = 0; i < sz; ++i) {
    y[i] = i;
    x[i] = 1;
  }
}

void print_results (float *y, const int sz, char *str)
{
  for (int i = 0; i < sz; ++i) {
    if (i > sz-5)
      printf ("%s y[%d]=%f\n", str, i, y[i]);
  }
  printf ("\n");
}
