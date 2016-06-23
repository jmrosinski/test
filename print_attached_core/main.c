#include <mpi.h>
#include <omp.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/syscall.h>

char status;  // status of thread
int core;    // prv or current core on which running
extern void run_forever (FILE *);
extern void print_whos_running_where (FILE *, int);

int main (int argc, char **argv)
{
  int iam;
  int nthreads, mythread;
  int ret;
  int n;
  int pid, tid;
  char path[64];
  FILE *fp;
  int c;
  int pin2core = 0;
  int pin2range = 0;

  ret = MPI_Init (&argc, &argv);
  ret = MPI_Comm_rank (MPI_COMM_WORLD, &iam);
  pid = getpid ();
  nthreads = omp_get_max_threads ();

  while ((c = getopt (argc, argv, "cr")) != -1) {
    switch (c) {
    case 'c':
      pin2core = 1;
      break;
    case 'r':
      pin2range = 1;
      break;
    default:
      printf ("unknown option %c\n", c);
      return 1;
      break;
    }
  }

  if (pin2core) {
    if (iam == 0) {
      printf ("Pinning threads to individual cores\n");
    }
    ret = set_affinity_ (&iam, &cpn, &nthreads, &pin2core);
  } else if (pin2range) {
    if (iam == 0) {
      printf ("Pinning threads to subsetted range of cores\n");
    }
    ret = set_affinity_ (&iam, &cpn, &nthreads, &pin2core);
  } else {
    if (iam == 0) {
      printf ("No pinning\n");
    }
  }
  ret = print_affinity (&iam);

#pragma omp parallel for private (fp, tid, core, mythread, status)
  for (n = 0; n < nthreads; ++n) {
    //    tid = gettid ();
#ifdef SYS_gettid
    tid = syscall (SYS_gettid);
#else
#error "SYS_gettid unavailable on this system"
#endif

    sprintf (path, "/proc/%d/task/%d/stat", pid, tid);
    fp = fopen (path, "r");
    fscanf (fp, "%*d %*s %c %*ld %*ld %*ld %*ld %*ld %*ld %*ld %*ld %*ld %*ld "
	                   "%*ld %*ld %*ld %*ld %*ld %*ld %*ld %*ld %*ld %*ld "
	                   "%*ld %*ld %*ld %*ld %*ld %*ld %*ld %*ld %*ld %*ld "
	                   "%*ld %*ld %*ld %*ld %*ld %d %*d %*d %*d %*d %*d", &status, &core);
    printf ("thread=%d status=%c core=%d\n", mythread, status, core);
    run_forever (fp);
  }
}

void run_forever (FILE *fp)
{
  int mythread = omp_get_thread_num ();

  while (1) {
    print_whos_running_where (fp, mythread);
    usleep (1000);
  }
}

void print_whos_running_where (FILE *fp, int mythread)
{
  char newstatus;
  int newcore;

  rewind (fp);
  fscanf (fp, "%*d %*s %c %*ld %*ld %*ld %*ld %*ld %*ld %*ld %*ld %*ld %*ld "
        	         "%*ld %*ld %*ld %*ld %*ld %*ld %*ld %*ld %*ld %*ld "
	                 "%*ld %*ld %*ld %*ld %*ld %*ld %*ld %*ld %*ld %*ld "
	                 "%*ld %*ld %*ld %*ld %*ld %d %*d %*d %*d %*d %*d", &newstatus, &newcore);
  if (newcore != core) {
    printf ("thread=%d status=%c core=%d\n", mythread, newstatus, newcore);
    status = newstatus;
    core = newcore;
  }
}
  
