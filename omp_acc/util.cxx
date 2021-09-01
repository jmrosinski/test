#include <math.h>
#include <stdio.h>
#include "dims.h"

extern float A[N][M];
extern float Anew[N][M];

void printdiff (struct timeval tp1, struct timeval tp2, const char *str)
{
  float diff = 1.e3*(tp2.tv_sec - tp1.tv_sec) + 1.e-3*(tp2.tv_usec - tp1.tv_usec);
  printf ("%f msec %s\n", diff, str);
}

void check_results (const char *str)
{
  int isave, jsave;
  float maxerr = -1.;;

  for (int j=1; j < N-1; ++j) {
    for (int i=1; i < M-1; ++i) {
      float anew = 0.25 * (A[j][i+1] + A[j][i-1] + A[j-1][i] + A[j+1][i]);
      float diff = fabs (Anew[j][i] - anew);
      if (diff > maxerr) {
	isave = i;
	jsave = j;
	maxerr = diff;
      }
    }
  }
  if (maxerr == 0.)
    printf ("%s maxerr=0.\n", str);
  else
    printf ("%s maxerr=%f at isave=%d jsave=%d A=%f %f %f %f\n", str, maxerr, isave, jsave,
	  A[jsave][isave+1], A[jsave][isave-1], A[jsave-1][isave], A[jsave+1][isave]);
}
