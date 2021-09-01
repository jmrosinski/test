#include <omp.h>
#include <sys/time.h>
#include "dims.h"

// Test code for stencil. Can try tiling
// Best to compile with C++ compiler due to multi-dim arrays--gdb can't handle under C

extern void printdiff (struct timeval, struct timeval, const char *);
extern void check_results (const char *);
float A[N][M];
float Anew[N][M];

int main ()
{
  int ret;
  struct timeval tp1, tp2;

  for( int j=0; j < N; ++j)
    for(int i=0; i < M; ++i)
      A[j][i] = i + j;

  ret = gettimeofday (&tp1, 0);
#pragma omp parallel for
  for (int j=1; j < N-1; ++j) {
#pragma omp simd
    for (int i=1; i < M-1; ++i) {
      Anew[j][i] = 0.25 * (A[j][i+1] + A[j][i-1] + A[j-1][i] + A[j+1][i]);
    }
  }
  ret = gettimeofday (&tp2, 0);
  printdiff (tp1, tp2, "omp_loop");
  check_results ("omp_loop");

  return 0;
}

