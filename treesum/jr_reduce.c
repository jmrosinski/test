#include <stdio.h>
#include <mpi.h>
#include <gptl.h>

int main (int argc, char **argv)
{
  const int tag = 98789;   // tag for MPI message
  const int niter = 1000; // number of iterations
  int iter;    // iterator
  int iam;     // my rank
  int ntask;   // task count
  int incr;    // increment for send/recv
  int sendto;  // taskto send to
  int recvfm;  // task to receive from
  int dosend;  // logical for whether to send this time step
  int dorecv;  // logical for whether to receive this time step
  int twoincr; // twice the current increment
  int ret;     // return code
  float sum;   // global sum
  float sum_p; // received partial sum
  float sum2;  // received partial sum
  MPI_Status status;   // required by MPI_Recv
  double value; // timing result

  ret = MPI_Init (&argc, &argv);
  ret = MPI_Comm_size (MPI_COMM_WORLD, &ntask);
  ret = MPI_Comm_rank (MPI_COMM_WORLD, &iam);

  if (iam == 0) {
    printf ("This program uses gptl to compare times for MPI_Reduce vs. the log(n) algorithm\n");
    printf ("Current setting is hard-wired to do MPI_SUM\n");
    printf ("There are %d tasks\n", ntask);
  }

  ret = GPTLinitialize ();

  ret = GPTLstart ("jr_reduce");
  for (iter = 0; iter < niter; ++iter) {
    sum = (float) iam;

    for (incr = 1; incr < ntask; incr = twoincr) {
      twoincr = 2*incr;
      sendto = iam - incr;
      recvfm = iam + incr;
      dorecv = ((iam + twoincr) % twoincr == 0) && (recvfm < ntask);
      dosend = ((iam + incr) % twoincr == 0) && (sendto > -1);
      if (dosend) {
	ret = MPI_Send (&sum, 1, MPI_FLOAT, sendto, tag, MPI_COMM_WORLD);
      } else if (dorecv) {
	ret = MPI_Recv (&sum_p, 1, MPI_FLOAT, recvfm, tag, MPI_COMM_WORLD, &status);
	sum += sum_p;
      }
    }
  }
  ret = GPTLstop ("jr_reduce");
  ret = MPI_Barrier (MPI_COMM_WORLD);
  if (iam == 0)
    printf ("jr_reduce produces sum=%f\n", sum);

  ret = GPTLstart ("MPI_Reduce");
  for (iter = 0; iter < niter; ++iter) {
    sum = (float) iam;
    ret = MPI_Reduce (&sum, &sum2, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);
  }
  ret = GPTLstop ("MPI_Reduce");

  if (iam == 0) {
    printf ("MPI_Reduce produces sum=%f\n", sum2);

    ret = GPTLget_wallclock ("jr_reduce", 0, &value);
    printf ("Time for jr_reduce=%g\n", value);

    ret = GPTLget_wallclock ("MPI_Reduce", 0, &value);
    printf ("Time for MPI_Reduce=%g\n", value);
  }

  ret = MPI_Finalize ();

  return 0;
}
