#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void do_nothing (int *);

void speedchk (int num, int one)
{
  int i, ii, j;
  int *arr;                 /* array for memory transfer rate */
  int sum0;
  int sum1;
  int sum2;           /* local sums */
  int sum3;
  int sum4;
  int sum5;
  int sum6;
  int sum7;
  int sum8;
  int sum9;
  float ratew;              /* computation rate (wallclock) */
  float ratew2;             /* computation rate (wallclock) */
  float ratec;              /* computation rate (CPU) */
  float deltaw;             /* change (wallclock) */
  float deltac;             /* change (CPU) */
  float inctime;            /* user time to increment loop */
  struct rusage r_usage1,   /* returned by getrusage */
                r_usage2;   /* returned by getrusage */
  double wall1, usr1, sys1;
  double wall2, usr2, sys2;

  sum0 = sum1 = sum2 = sum3 = sum4 = sum5 = sum6 = sum7 = 0;

  (void) t_stamp (&wall1, &usr1, &sys1);

  for (i = 0; i < num; i += one) {
    sum0 += i;
    sum1 += sum0;
    sum2 += sum1;
    sum3 += sum2;
    sum4 += sum3;
    sum5 += sum4;
    sum6 += sum5;
    sum7 += sum6;
  }

  (void) t_stamp (&wall2, &usr2, &sys2);

  do_nothing (&sum7);
  deltaw = wall2 - wall1;
  deltac = (usr2 + sys2) - (usr1 + sys1);

  /*
  ** 8 things are done, + 1 for i increment + 1 for test
  */

  ratew = (num * 10.) / deltaw;
  ratec = (num * 10.) / deltac;

  printf ("deltaw = %f Clock rate estimate (wall): %f MHz\n", 
	  deltaw, ratew * 1.e-6);
  
  printf ("deltac = %f Clock rate estimate (cpu ): %f MHz\n", 
	  deltaw, ratec * 1.e-6);

  sum0 = sum1 = sum2 = sum3 = sum4 = sum5 = sum6 = sum7 = 0;

  (void) t_stamp (&wall1, &usr1, &sys1);
  
  for (i = 0; i < num; i += one) {
    sum0 += i*num;
    sum1 += sum0*num;
    sum2 += sum1*num;
    sum3 += sum2*num;
    sum4 += sum3*num;
    sum5 += sum4*num;
    sum6 += sum5*num;
    sum7 += sum6*num;
  }

  (void) t_stamp (&wall2, &usr2, &sys2);
  
  do_nothing (&sum7);
  deltaw = wall2 - wall1;
  deltac = (usr2 + sys2) - (usr1 + sys1);

  /*
  ** subtract inctime to eliminate loop indexing overhead
  */

  ratew2 = (num * 10.) / deltaw;

  printf ("deltaw = %f Mult/add factor is %f\n", deltaw, ratew2 / ratew);

  if ((arr = malloc (num * sizeof (int))) == NULL) {
    printf ("cannot malloc %d words\n", num);
    return;
  }

  (void) t_stamp (&wall1, &usr1, &sys1);

  for (i = 0; i < num; i++) {
    arr[i] = i;
  }

  (void) t_stamp (&wall2, &usr2, &sys2);

  do_nothing (arr);
  deltaw = wall2 - wall1;
  deltac = (usr2 + sys2) - (usr1 + sys1);

  /*
  ** Try to factor out time to increment loop index
  */

  ratew = num / deltaw;
  ratec = num / deltac;

  printf ("deltaw = %f Memory write rate estimate: %f MHz\n", deltaw, 
	  ratew * 1.e-6);

  printf ("deltac = %f Memory write rate estimate: %f MHz\n", deltac, 
	  ratec * 1.e-6);

  free (arr);

  return;
}

void do_nothing (int *arr)
{
  return;
}
