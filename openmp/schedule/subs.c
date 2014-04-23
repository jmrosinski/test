#include <omp.h>
#include <unistd.h>

void load_balanced_sub_ (int *niter, int *n, int *whodoneit, int *kount)
{
  int mythread = omp_get_thread_num ();
  int ret;

  ret = usleep ((*niter)/2); /* everyone sleeps the same amount */
  whodoneit[*n] = mythread;
  ++kount[mythread];
}

void load_unbalanced_sub_ (int *n, int *whodoneit, int *kount)
{
  int mythread = omp_get_thread_num ();
  int ret;

  ret = usleep (*n); /* sleep n microseconds */
  whodoneit[*n] = mythread;
  ++kount[mythread];
}
