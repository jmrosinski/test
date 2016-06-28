#include <mpi.h>
#include <omp.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/syscall.h>

int iam;
int shiftiam;  // for shifting affinity
int nranks;
int pid;       // process id
int nthreads;
int shiftintvl = 10;    // wait this long before forcing affinity shift (default 10 seconds)
volatile int *core;     // array of attached cores
extern void init_core (int);
extern void threaded_loop (void);
extern void print_whos_running_where (FILE *, int);

int main (int argc, char **argv)
{
  int ret;
  FILE *fp;
  int c;
  int pin2core = 0;    // 1=> pin threads to single core
  int pin2range = 0;   // 1=> pin threads to range of cores
  int cpn = 1;         // default cores per node

  ret = MPI_Init (&argc, &argv);
  ret = MPI_Comm_rank (MPI_COMM_WORLD, &iam);
  shiftiam = iam;
  ret = MPI_Comm_size (MPI_COMM_WORLD, &nranks);

  pid = getpid ();
  nthreads = omp_get_max_threads ();
  core = malloc (nthreads * sizeof (int));
  init_core (nthreads);

  while ((c = getopt (argc, argv, "hcrn:w:")) != -1) {
    switch (c) {
    case 'h':
      if (iam == 0) {
	printf ("Usage: %s [-c] to pin to single core\n"
		"          [-r] to pin to range of cores\n"
		"          [-n <num>] number of cores per node\n"
		"          [-w <num> number of seconds between forced shifts", argv[0]);
      }
      return 0;
      break;
    case 'c':
      pin2core = 1;
      break;
    case 'r':
      pin2range = 1;
      break;
    case 'n':
      cpn = atoi (optarg);
      break;
    case 'w':
      shiftintvl = atoi (optarg);
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
  ret = print_affinity_ (&iam);

  while (1) {
    // Loop for 1 second and print when attachment changes
    threaded_loop ();
    // Change affinity to guarantee all is working as expected
    if (pin2core || pin2range) {
      shiftiam = (shiftiam + 1) % nranks;
      printf ("shiftiam=%d\n", shiftiam);
      ret = set_affinity_ (&shiftiam, &cpn, &nthreads, &pin2core);
    }
    ret = print_affinity_ (&iam);
  }
}

void threaded_loop ()
{
  int n;
  int iter;
  int ret;
  int tid;
  int mythread;
  static const int usleeptime = 100000;
  int niter = ((unsigned long) shiftintvl * 1000000) / usleeptime;
  char path[64];
  FILE *fp;

  // Loop for shiftintvl seconds and print out if any affinities change
#pragma omp parallel for private (fp, path, tid, mythread)
  for (n = 0; n < nthreads; ++n) {
    mythread = omp_get_thread_num ();
#ifdef SYS_gettid
    tid = syscall (SYS_gettid);
#else
#error "SYS_gettid unavailable on this system"
#endif

    sprintf (path, "/proc/%d/task/%d/stat", pid, tid);
    fp = fopen (path, "r");
      
    for (iter = 0; iter < niter; ++iter) {
      print_whos_running_where (fp, mythread);
      usleep (usleeptime);
    }
    if (fclose (fp)) {
      printf ("Failed to close path=%s\n", path);
      ret = MPI_Abort (MPI_COMM_WORLD, -1);
    }
  }
}

void print_whos_running_where (FILE *fp, int mythread)
{
  char status;
  int newcore;

  rewind (fp);
  fscanf (fp, "%*d %*s %c %*ld %*ld %*ld %*ld %*ld %*ld %*ld %*ld %*ld %*ld "
        	         "%*ld %*ld %*ld %*ld %*ld %*ld %*ld %*ld %*ld %*ld "
	                 "%*ld %*ld %*ld %*ld %*ld %*ld %*ld %*ld %*ld %*ld "
	                 "%*ld %*ld %*ld %*ld %*ld %d %*d %*d %*d %*d %*d", &status, &newcore);
  if (newcore != core[mythread]) {
    printf ("iam=%d thread=%d status=%c core=%d\n", iam, mythread, status, newcore);
    core[mythread] = newcore;
  }
}

void init_core (int nthreads)
{
  int n;

  for (n = 0; n < nthreads; ++n)
    core[n] = -1;
}
