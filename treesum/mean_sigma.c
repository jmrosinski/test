#include <stdio.h>
#include <math.h>

int main (int argc, char **argv)
{
  int n;
  int i;
  float mean;
  float sigma;
  float x;
  float arr[100];

  printf ("This program takes user input of up to 100 numbers and computes mean and\n");
  printf ("standard deviation using the naive 2-pass algorithm\n");
  n = 0;
  while (1) {
    printf ("Enter a floating point number (-999. when done)\n");
    scanf ("%f", &x);
    if (x == -999.)
      break;
    arr[n++] = x;
  }
  printf ("Processing %d numbers\n", n);

  mean = 0.;
  for (i = 0; i < n; ++i)
    mean += arr[i];
  mean /= n;

  sigma = 0.;
  for (i = 0; i < n; ++i)
    sigma += (arr[i] - mean)*(arr[i] - mean);
  sigma = sqrt(sigma/(n-1));

  printf ("mean=%f sigma=%f\n", mean, sigma);

  return 0;
}
