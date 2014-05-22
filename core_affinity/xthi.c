#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sched.h>
#include <mpi.h>
#include <omp.h>
#include <unistd.h>  /* getopt */

/* Borrowed from util-linux-2.13-pre7/schedutils/taskset.c */
static char *cpuset_to_cstr(cpu_set_t *mask, char *str)
{
  char *ptr = str;
  int i, j, entry_made = 0;
  for (i = 0; i < CPU_SETSIZE; i++) {
    if (CPU_ISSET(i, mask)) {  // this thread/task can run on core i
      int run = 0;
      entry_made = 1;
      for (j = i + 1; j < CPU_SETSIZE; j++) { 
        if (CPU_ISSET(j, mask)) run++;
        else break;
      }
      if (!run)                 // run=0 means cannot run on any subsequent cores
        sprintf(ptr, "%d,", i);
      else if (run == 1) {      // run=1 means just core i and i+1 are runnable
        sprintf(ptr, "%d,%d,", i, i + 1);
        i++;
      } else {                  // run>1 means there's a range of runnable cores
        sprintf(ptr, "%d-%d,", i, i + run);
        i += run;
      }
      while (*ptr != 0) ptr++;
    }
  }
  ptr -= entry_made;
  *ptr = 0;
  return(str);
}

int main(int argc, char *argv[])
{
  int rank, thread;
  cpu_set_t coremask;
  char clbuf[7 * CPU_SETSIZE], hnbuf[64];

  int c;        //JR for spinning
  int spin = 0; //JR for spinning: default is do not

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  //JR begin additions for spinning
  while ((c = getopt (argc, argv, "s")) != -1) {
    switch (c) {
    case 's':
      spin = 1;
      break;
    default:
      printf ("unknown option %c\n", c);
      return 1;
      break;
    }
  }

  memset(clbuf, 0, sizeof(clbuf));
  memset(hnbuf, 0, sizeof(hnbuf));
  (void)gethostname(hnbuf, sizeof(hnbuf));
#pragma omp parallel private(thread, coremask, clbuf)
  {
    thread = omp_get_thread_num();
    //JR Passing zero means use the calling process
    (void)sched_getaffinity(0, sizeof(coremask), &coremask);
    cpuset_to_cstr(&coremask, clbuf);
    #pragma omp barrier
    printf("Hello from rank %d, thread %d, on %s. (core affinity = %s)\n",
	   rank, thread, hnbuf, clbuf);
    //JR Put in a spin loop
    if (spin)
      while (1);
  }
  MPI_Finalize();
  return(0);
}
