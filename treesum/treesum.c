#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <mpi.h>

enum Sumtype {CONSTANT, IAM};  // Distribution choices are: each=CONSTANT or each=IAM
enum Sumtype sumtype = IAM;    // Make the choice here
const float thingtosum = 0.1;  // Can make the constant whatever you want

int main (int argc, char **argv)
{
  int iam;     // my rank
  int ntask;   // task count
  int step;    // step number
  int incr;    // increment for send/recv
  int sendto;  // taskto send to
  int recvfm;  // task to receive from
  int dosend;  // logical for whether to send this time step
  int dorecv;  // logical for whether to receive this time step
  int twoincr; // twice the current increment
  int ret;     // return code
  float sum;   // global sum using MPI
  float sum2;  // global sum computed analytically
  float sum_p; // received partial sum
  static const int tag = 98789;                    /* tag for MPI message */
  MPI_Status status;   // required by MPI_Recv
  int naccum;          // initial value: rank owns itself
  int naccum_p;
  int temp;            // temporary
  float m2;
  float m2_p;
  float mean;
  float mean_p;
  float delta;
  float r;
  float sigma;

  float linearsum (int);
  int mean_sigma (int, float *, float *);

  ret = MPI_Init (&argc, &argv);
  ret = MPI_Comm_size (MPI_COMM_WORLD, &ntask);
  ret = MPI_Comm_rank (MPI_COMM_WORLD, &iam);

  if (iam == 0) {
    printf ("This program uses MPI to compute a tree sum and mean and st. dev. stats.\n");
    printf ("Mean and st. dev stats are then compared to a naive 2-pass algorithm\n");
    printf ("There are %d tasks\n", ntask);
    if (sumtype == CONSTANT)
      printf ("Current setting is all values=%f\n", thingtosum);
    else
      printf ("Current setting is each value=myrank\n");
  }

  if (sumtype == CONSTANT)
    sum = thingtosum;
  else
    sum = (float) iam;    
  mean = sum;
  naccum = 1;
  m2 = 0.;
  for (incr = 1; incr < ntask; incr = twoincr) {
    twoincr = 2*incr;
    sendto = iam - incr;
    recvfm = iam + incr;
    dorecv = ((iam + twoincr) % twoincr == 0) && (recvfm < ntask);
    dosend = ((iam + incr) % twoincr == 0) && (sendto > -1);
    if (dosend) {
      assert (! dorecv);
      ret = MPI_Send (&sum, 1, MPI_FLOAT, sendto, tag, MPI_COMM_WORLD);
      ret = MPI_Send (&naccum, 1, MPI_INT, sendto, tag, MPI_COMM_WORLD);
      ret = MPI_Send (&mean, 1, MPI_FLOAT, sendto, tag, MPI_COMM_WORLD);
      ret = MPI_Send (&m2, 1, MPI_FLOAT, sendto, tag, MPI_COMM_WORLD);
    }
    if (dorecv) {
      assert (! dosend);
      ret = MPI_Recv (&sum_p, 1, MPI_FLOAT, recvfm, tag, MPI_COMM_WORLD, &status);
      ret = MPI_Recv (&naccum_p, 1, MPI_INT, recvfm, tag, MPI_COMM_WORLD, &status);
      ret = MPI_Recv (&mean_p, 1, MPI_FLOAT, recvfm, tag, MPI_COMM_WORLD, &status);
      ret = MPI_Recv (&m2_p, 1, MPI_FLOAT, recvfm, tag, MPI_COMM_WORLD, &status);
      sum += sum_p;
      // Compute mean, variance
      delta = mean_p - mean;
      mean += delta*naccum_p/(naccum_p+naccum);
      m2 += m2_p + delta*delta*(naccum_p*naccum)/(naccum_p+naccum);
      naccum += naccum_p;
    }
    ++step;
  }

  if (iam == 0) {
    printf ("sum from MPI=%f\n", sum);
    sum2 = linearsum (ntask);
    printf ("sum from naive calc =%f\n", sum2);
    
    sigma = sqrt (m2/(naccum-1));
    printf ("mean, sigma from MPI=%g %g\n", mean, sigma);
    ret = mean_sigma (ntask, &mean, &sigma);
    printf ("mean, sigma from 2-pass simplest algorithm=%g %g\n", mean, sigma);
  }
  ret = MPI_Finalize ();
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
