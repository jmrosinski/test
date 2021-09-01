#include <openacc.h>
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
#pragma acc enter data create (Anew) copyin (A)
  ret = gettimeofday (&tp2, 0);
  printdiff (tp1, tp2, "enter_data");

  ret = gettimeofday (&tp1, 0);
  // Tiling with g++ gives wrong answers nvc++ works ok
//#pragma acc parallel loop tile(32,4)
#pragma acc parallel num_workers(4) vector_length(32)
  //#pragma acc parallel
  {
#pragma acc loop gang
    for (int j=1; j < N-1; ++j) {
#pragma acc loop worker vector
      for (int i=1; i < M-1; ++i) {
//#pragma acc cache (A[j][0:M], A[j-1][1:M-1], A[j+1][1:M-1])
//#pragma acc cache (A[j][0:M])
	Anew[j][i] = 0.25 * (A[j][i+1] + A[j][i-1] + A[j-1][i] + A[j+1][i]);
      }
    }
  }
  ret = gettimeofday (&tp2, 0);
  printdiff (tp1, tp2, "parallel_loop");

  ret = gettimeofday (&tp1, 0);
#pragma acc exit data copyout (Anew)
  ret = gettimeofday (&tp2, 0);
  printdiff (tp1, tp2, "exit_data");
  check_results ("exit_data");

  return 0;
}
