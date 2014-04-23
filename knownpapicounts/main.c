#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <gptl.h>

#ifdef PAPI
#include <papi.h>
#endif

int main (int argc, char **argv)
{
  char c;
  unsigned long looplen = 1000000;
  unsigned long i;
  unsigned long nbytes;

  double *arr;

  int cpu_fpmadd;
  int cpu_fparith;
  int ret;

  double add (unsigned long, double *);
  double multadd (unsigned long, double *);
  double compare (unsigned long, double *);

  while ((c = getopt (argc, argv, "l:")) != -1) {
    switch (c) {
      case 'l':
	looplen = atol (optarg);
	printf ("repeat count: %lu\n", looplen);
	break;
      default:
	printf ("unknown option %c\n", c);
	return 1;
	break;
    }
  }

  nbytes = looplen * sizeof (double);
  if ( ! (arr = malloc (nbytes))) {
    printf ("Can't malloc %lu doubles\n", looplen);
    return -1;
  }

  memset (arr, 0, nbytes);

#ifdef PAPI
  ret = PAPI_event_name_to_code ("CPU_FPMADD", &cpu_fpmadd);
  ret = PAPI_event_name_to_code ("CPU_FPARITH", &cpu_fparith);
  GPTLsetoption (cpu_fpmadd,1);
  GPTLsetoption (cpu_fparith,1);
#endif

  GPTLinitialize ();
  GPTLstart ("total");

  (void) add (looplen, arr);
  (void) multadd (looplen, arr);
  (void) compare (looplen, arr);

  GPTLstop ("total");
  GPTLpr (0);
}

double add (unsigned long looplen, double *arr)
{
  unsigned long i;
  char string[128];
  double sum = 0.;

  if (looplen < 1000000)
    sprintf (string, "%luadditions", looplen);
  else
    sprintf (string, "%-.3gadditions", (double) looplen);

  if (GPTLstart (string) < 0)
    exit (1);

  for (i = 0; i < looplen; ++i)
    sum += arr[i];

  if (GPTLstop (string) < 0)
    exit (1);

  return sum;
}

double multadd (unsigned long looplen, double *arr)
{
  unsigned long i;
  char string[128];
  double sum = 0.;

  if (looplen < 1000000)
    sprintf (string, "%lumultadds", looplen);
  else
    sprintf (string, "%-.3gmultadds", (double) looplen);

  if (GPTLstart (string) < 0)
    exit (1);

  for (i = 0; i < looplen; ++i)
    sum += 1.1*arr[i];

  if (GPTLstop (string) < 0)
    exit (1);

  return sum;
}

double compare (unsigned long looplen, double *arr)
{
  unsigned long i;
  char string[128];
  double val = 0.;

  if (looplen < 1000000)
    sprintf (string, "%lucompares", looplen);
  else
    sprintf (string, "%-.3gcompares", (double) looplen);

  if (GPTLstart (string) < 0)
    exit (1);

  for (i = 0; i < looplen; ++i)
    if (arr[i] < -1.)
      arr[i] = 0.;

  if (GPTLstop (string) < 0)
    exit (1);

  return val;
}
