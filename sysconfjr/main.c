#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <stdlib.h>
#include <string.h>         /* strerror */
#include <errno.h>
#include <stdio.h>

#if ( defined LINUX )
#include <linux/kernel.h>
#endif

#if ( defined SGI )

#define NPCONF _SC_NPROC_CONF
#define NPONLN _SC_NPROC_ONLN

#elif ( defined CRAY )

#define NPCONF _SC_CRAY_NCPU
#define NPONLN _SC_CRAY_NCPU

#endif

#define FALSE 0
#define TRUE 1
#define ONEKB 1024
static const int onemb = ONEKB * ONEKB;

void speedchk (int, int);

main(int argc, char **argv)
{
  void *stuff;
  int nbytes, mbytes, c;
  int n, ncpus, nf;
  int memcheck = FALSE;
  int error;
  int ret;
  struct rusage usage;

#ifndef CRAY
  struct rlimit rlp;
#endif

  while ((c = getopt(argc,argv,"m")) != -1) {
    switch(c) {
    case 'm':
      memcheck = TRUE;
      break;
    default:
      printf ("Unknown option %c encountered\n", c);
    }
  }

  printf("Sizeof pointer                = %d\n", (int) sizeof (void *));
  printf("Sizeof short                  = %d\n", (int) sizeof (short));
  printf("Sizeof int                    = %d\n", (int) sizeof (int));
  printf("Sizeof long                   = %d\n", (int) sizeof (long));
  printf("Sizeof long long              = %d\n", (int) sizeof (long long));
  printf("Sizeof double                 = %d\n", (int) sizeof (double));
  printf("Sizeof long double            = %d\n", (int) sizeof (long double));
  printf("Max size of argv[] + envp[]   = %lu\n",(unsigned long) sysconf(_SC_ARG_MAX));
  printf("Max processes allowed any uid = %lu\n",(unsigned long) sysconf(_SC_CHILD_MAX));
  printf("Clock ticks per second =        %lu\n",(unsigned long) sysconf(_SC_CLK_TCK));
  printf("Max open files per process =    %lu\n",(unsigned long) sysconf(_SC_OPEN_MAX));
  printf("Version of POSIX.1 =            %lu\n",(unsigned long) sysconf(_SC_VERSION));

#if ( defined CRAY ) || ( defined SGI )
  printf("Number of CPUs configured =     %lu\n",(unsigned long) sysconf(NPCONF));
  ncpus = sysconf(NPONLN);
  printf("Number of CPUs online =         %d\n",ncpus);
#endif

  printf("Page size =                     %lu\n",(unsigned long) sysconf(_SC_PAGESIZE));

  getrlimit (RLIMIT_AS, &rlp);
  printf ("Soft limit on virtual process size = %lu MB\n", (unsigned long) rlp.rlim_cur / onemb);
  printf ("Hard limit on virtual process size = %lu MB\n", (unsigned long) rlp.rlim_max / onemb);
	     
  if ( getrlimit (RLIMIT_CORE, &rlp) != 0) {
    printf ("Bad return from getrlimit\n");
    exit (1);
  }
  printf ("Soft limit on max core filesize = %lu MB\n", (unsigned long) rlp.rlim_cur / onemb);
  printf ("Hard limit on max core filesize = %lu MB\n", (unsigned long) rlp.rlim_max / onemb);

  getrlimit (RLIMIT_CPU, &rlp);
  printf ("Soft limit on max CPU usage     = %lu \n", (unsigned long) rlp.rlim_cur);
  
  getrlimit (RLIMIT_DATA, &rlp);
  printf ("Soft limit on max HEAP usage    = %lu MB\n", (unsigned long) rlp.rlim_cur / onemb);
  printf ("Hard limit on max HEAP usage    = %lu MB\n", (unsigned long) rlp.rlim_max / onemb);
  
  getrlimit (RLIMIT_FSIZE, &rlp);
  printf ("Soft limit on max file size     = %lu MB\n", (unsigned long) rlp.rlim_cur / onemb);
  printf ("Hard limit on max file size     = %lu MB\n", (unsigned long) rlp.rlim_max / onemb);
  
  getrlimit (RLIMIT_STACK, &rlp);
  printf ("Soft limit on max stack size    = %lu MB\n", (unsigned long) rlp.rlim_cur / onemb);
  printf ("Hard limit on max stack size    = %lu MB\n", (unsigned long) rlp.rlim_max / onemb);

#if ( defined SGI )
  getrlimit (RLIMIT_RSS, &rlp);
  printf ("Soft limit on max RSS           = %lu MB\n", (unsigned long) rlp.rlim_cur / onemb);
  printf ("Hard limit on max RSS           = %lu MB\n", (unsigned long) rlp.rlim_max / onemb);
  getrlimit (RLIMIT_VMEM, &rlp);
  printf ("Soft limit on max virtual memory= %lu MB\n", (unsigned long) rlp.rlim_cur / onemb);
  printf ("Hard limit on max virtual memory= %lu MB\n", (unsigned long) rlp.rlim_max / onemb);
#endif

  ret = getrusage (RUSAGE_SELF, &usage);
  if (usage.ru_maxrss == 0)
    printf ("maxrss==0 so probably getrusage does not return meaningful maxrss\n");
  else
    printf ("maxrss==%lu so getrusage returns meaningful maxrss\n", (unsigned long) usage.ru_maxrss);

  if (memcheck) {
    for (nbytes = 1000000; ;nbytes *= 1.1) {
      if ((stuff = malloc ((size_t) nbytes)) == NULL) break;
      free (stuff);
    }
    mbytes = nbytes/1.1e6;
    printf ("Max mallocable memory = %d MB within 10%%\n", mbytes);
  }

  exit(0);
}
