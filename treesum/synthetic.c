#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>

enum Sumtype {CONSTANT, IAM};  // Distribution choices are: each=CONSTANT or each=IAM
enum Sumtype sumtype = IAM;    // Make the choice here
const float thingtosum = 0.1;  // Can make the constant whatever you want

int main ()
{
  int i;       // loop index
  int iam;     // simulated MPI rank 
  int ntask;   // simulated number of MPI tasks
  int step;    // step indicator
  int incr;
  int sendto;
  int recvfm;
  int dosend;
  int dorecv;
  int twoincr;
  int ret;

  float *sum;   /* array of ntask intermediate sums */
  float *mean;  /* array of ntask intermediate means */
  int *naccum;  /* array of ntask intermediate number of accumulations */
  float delta;  /* intermediate difference between means */
  float sigma;  /* std deviation using MPI simulation */

  float sum2;   /* sum computed linearly */
  float mean2;  /* mean using naive 2-pass calculation */
  float *m2;    /* array of ntask intermediate M2 calcs */
  float sigma2; /* std deviation using naive 2-pass calculation */
  int tsksum;

  float linearsum (int);
  int mean_sigma (int, float *, float *);
  int mean_sigma_double (int, float *, float *);

  printf ("This program simulates with a single task what 'treesum' would do with MPI tasks.\n");
  printf ("It compares a tree algorithm vs. a naive 2-pass algorithm to compute mean and \n");
  printf ("st. dev. stats");
  if (sumtype == CONSTANT)
    printf ("Current setting is all values=%f\n", thingtosum);
  else
    printf ("Current setting is each value=myrank\n");

  while (1) {
    printf ("Enter ntask\n");
    scanf ("%d", &ntask);
    printf ("ntask=%d:\n", ntask);
    if (ntask < 1)
      break;

    sum    = (float *) malloc (ntask * sizeof (float));
    mean   = (float *) malloc (ntask * sizeof (float));
    naccum = (int *)   malloc (ntask * sizeof (int));
    m2     = (float *) malloc (ntask * sizeof (float));

    step = 1;
    for (i = 0; i < ntask; ++i) {
      if (sumtype == CONSTANT) {
	sum[i] = thingtosum;
      } else {
	sum[i] = (float) i;
      }
      mean[i] = sum[i];
      naccum[i] = 1;
      m2[i] = 0.;
    }

    for (incr = 1; incr < ntask; incr = twoincr) {
      twoincr = 2*incr;
      printf ("step=%d\n", step);
      for (iam = 0; iam < ntask; ++iam) {
	sendto = iam - incr;
	recvfm = iam + incr;
	dorecv = ((iam + twoincr) % twoincr == 0) && (recvfm < ntask);
	dosend = ((iam + incr) % twoincr == 0) && (sendto > -1);
	if (dosend) {
	  printf (" iam=%d sending to %d\n", iam, sendto);
	  assert (! dorecv);
	}
	if (dorecv) {
	  printf (" iam=%d recving fm %d\n", iam, recvfm);
	  assert (! dosend);

	  sum[iam] += sum[recvfm];
	  // Compute mean, variance
	  // [recvfm] refs. simulate the "received from" settings 
	  // in treesum for which variables end in "_p"
	  tsksum = naccum[recvfm] + naccum[iam];
	  delta = mean[recvfm] - mean[iam];
	  mean[iam] += (delta*naccum[recvfm])/tsksum;

	  // Need to cast to float to avoid integer overflow
	  m2[iam] += m2[recvfm] + delta*delta*((float) naccum[recvfm] * naccum[iam])/tsksum;
	  naccum[iam] = tsksum;
	}
      }
      ++step;
    }
    printf ("sum from MPI simulation=%g\n", sum[0]);
    sum2 = linearsum (ntask);
    printf ("sum from naive calc =%g\n", sum2);
    
    if (ntask < 2)
      sigma = 0.;
    else
      sigma = sqrt (m2[0]/(naccum[0]-1));

    printf ("mean, sigma from MPI simulation=%g %g\n", mean[0], sigma);
    ret = mean_sigma (ntask, &mean2, &sigma2);
    printf ("mean, sigma from 2-pass simplest algorithm=%g %g\n", mean2, sigma2);
    ret = mean_sigma_double (ntask, &mean2, &sigma2);
    printf ("mean, sigma from 2-pass simplest algorithm using doubles=%g %g\n", mean2, sigma2);

    // Free space for next test
    free (sum);
    free (mean);
    free (naccum);
    free (m2);
  }
  return 0;
}

float linearsum (int ntask)
{
  float sum = 0;
  int i;

  for (i = 0; i < ntask; ++i)
    if (sumtype == CONSTANT)
      sum += thingtosum;
    else
      sum += (float) i;

  return sum;
}

int mean_sigma (int ntask, float *mean_out, float *sigma_out)
{
  int i;
  float mean = 0.;
  float sigma = 0.;

  mean = 0.;
  for (i = 0; i < ntask; ++i)
    if (sumtype == CONSTANT)
      mean += thingtosum;
    else
      mean += i;

  mean /= ntask;

  sigma = 0.;
  for (i = 0; i < ntask; ++i)
    if (sumtype == CONSTANT)
      sigma += (thingtosum - mean)*(thingtosum - mean);
    else
      sigma += (i - mean)*(i - mean);

  sigma = sqrt(sigma/(ntask-1));

  *mean_out = mean;
  *sigma_out = sigma;
  return 0;
}

int mean_sigma_double (int ntask, float *mean_out, float *sigma_out)
{
  int i;
  double mean = 0.;
  double sigma = 0.;

  mean = 0.;
  for (i = 0; i < ntask; ++i)
    if (sumtype == CONSTANT)
      mean += thingtosum;
    else
      mean += i;

  mean /= ntask;

  sigma = 0.;
  for (i = 0; i < ntask; ++i)
    if (sumtype == CONSTANT)
      sigma += (thingtosum - mean)*(thingtosum - mean);
    else
      sigma += (i - mean)*(i - mean);

  sigma = sqrt(sigma/(ntask-1));

  *mean_out = mean;
  *sigma_out = sigma;
  return 0;
}
