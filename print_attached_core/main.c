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
int shiftintvl = 10;      // wait this long before forcing affinity shift (default 10 seconds)
int *core;       // array of attached cores
char *status;    // array of nthreads statuses
volatile int *tidarr;     // array of tids
FILE **fp;       // array of file pointers into /proc

extern void fill_tid_fp (void);
extern void init_core (int);
extern void threaded_loop (void);
extern void print_any_changes (void);
extern void print_all_statuses (void);

int main (int argc, char **argv)
{
  int ret;
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
  core   = malloc (nthreads * sizeof (int));
  status = malloc (nthreads * sizeof (char));
  tidarr = malloc (nthreads * sizeof (int));
  fp     = malloc (nthreads * sizeof (FILE *));
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
  fill_tid_fp ();

  while (1) {
    // Loop for some time (default 10 seconds), printing any core attachment changes
    threaded_loop ();
    // Change affinity to guarantee all is working as expected
    if (pin2core || pin2range) {
      shiftiam = (shiftiam + 1) % nranks;
      printf ("shiftiam=%d\n", shiftiam);
      ret = set_affinity_ (&shiftiam, &cpn, &nthreads, &pin2core);
    }
    ret = print_affinity_ (&iam);
    print_all_statuses ();
  }
}

void fill_tid_fp ()
{
  int n;
  int tid;
  int mythread;
  char path[64];

#pragma omp parallel for private (mythread, tid)
  for (n = 0; n < nthreads; ++n) {
    mythread = omp_get_thread_num ();
#ifdef SYS_gettid
    tid = syscall (SYS_gettid);
#else
#error "SYS_gettid unavailable on this system"
#endif
    tidarr[mythread] = tid;
  }

  // Master opens and reads the status from /proc
  if (omp_get_thread_num () == 0) {
    for (n = 0; n < nthreads; ++n) {
      sprintf (path, "/proc/%d/task/%d/stat", pid, tidarr[n]);
      fp[n] = fopen (path, "r");
    }
  }
}

void threaded_loop ()
{
  int n;
  int iter;
  int mythread;
  static const int usleeptime = 100000;
  int niter = ((unsigned long) shiftintvl * 1000000) / usleeptime;

  // Loop for shiftintvl seconds and print out if any affinities change
#pragma omp parallel for private (mythread)
  for (n = 0; n < nthreads; ++n) {
    mythread = omp_get_thread_num ();

    for (iter = 0; iter < niter; ++iter) {
      if (mythread == 0)
	print_any_changes ();

      usleep (usleeptime);
    }
  }
}

void print_any_changes ()
{
  char newstatus;
  int newcore;
  int n;

  for (n = 0; n < nthreads; ++n) {
    rewind (fp[n]);
    fscanf (fp[n], "%*d %*s %c %*ld %*ld %*ld %*ld %*ld %*ld %*ld %*ld %*ld %*ld "
        	              "%*ld %*ld %*ld %*ld %*ld %*ld %*ld %*ld %*ld %*ld "
	                      "%*ld %*ld %*ld %*ld %*ld %*ld %*ld %*ld %*ld %*ld "
	                      "%*ld %*ld %*ld %*ld %*ld %d %*d %*d %*d %*d %*d", 
	    &newstatus, &newcore);
    if (newcore != core[n]) {
      printf ("CORE CHANGE: iam=%d thread=%d core=%d\n", iam, n, newcore);
      core[n] = newcore;
    }
    if (newstatus != status[n]) {
      printf ("STATUS CHANGE: iam=%d thread=%d status=%c\n", iam, n, newstatus);
      status[n] = newstatus;
    }
  }
}

void print_all_statuses ()
{
  char newstatus;
  int newcore;
  int n;

  for (n = 0; n < nthreads; ++n) {
    rewind (fp[n]);
    fscanf (fp[n], "%*d %*s %c %*ld %*ld %*ld %*ld %*ld %*ld %*ld %*ld %*ld %*ld "
        	              "%*ld %*ld %*ld %*ld %*ld %*ld %*ld %*ld %*ld %*ld "
	                      "%*ld %*ld %*ld %*ld %*ld %*ld %*ld %*ld %*ld %*ld "
	                      "%*ld %*ld %*ld %*ld %*ld %d %*d %*d %*d %*d %*d", 
	    &newstatus, &newcore);
    printf ("print_all_statuses: iam=%d thread=%d core=%d status=%c\n", 
	    iam, n, newcore, newstatus);
  }
  printf ("\n");
}

void init_core (int nthreads)
{
  int n;

  for (n = 0; n < nthreads; ++n) {
    core[n] = -1;
    status[n] = 'x';
    tidarr[n] = -1;
  }
}
