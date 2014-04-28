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

#define FALSE 0
#define TRUE 1
#define ONEKB 1024UL
static const unsigned int onemb = ONEKB * ONEKB;
static const unsigned long onegb = ONEKB * ONEKB * ONEKB;

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

  printf("Number of CPUs configured =     %lu\n",(unsigned long) sysconf(_SC_NPROCESSORS_CONF));
  printf("Number of CPUs online =         %lu\n",(unsigned long) sysconf(_SC_NPROCESSORS_ONLN));

  printf("Page size =                     %lu\n",(unsigned long) sysconf(_SC_PAGESIZE));

  getrlimit (RLIMIT_AS, &rlp);
  printf ("Soft limit on virtual process size = %lu GB\n", (unsigned long) rlp.rlim_cur / onegb);
  printf ("Hard limit on virtual process size = %lu GB\n", (unsigned long) rlp.rlim_max / onegb);
	     
  if ( getrlimit (RLIMIT_CORE, &rlp) != 0) {
    printf ("Bad return from getrlimit\n");
    exit (1);
  }
  printf ("Soft limit on max core filesize = %lu GB\n", (unsigned long) rlp.rlim_cur / onegb);
  printf ("Hard limit on max core filesize = %lu GB\n", (unsigned long) rlp.rlim_max / onegb);

  getrlimit (RLIMIT_CPU, &rlp);
  printf ("Soft limit on max CPU usage     = %lu \n", (unsigned long) rlp.rlim_cur);
  
  getrlimit (RLIMIT_DATA, &rlp);
  printf ("Soft limit on max HEAP usage    = %lu GB\n", (unsigned long) rlp.rlim_cur / onegb);
  printf ("Hard limit on max HEAP usage    = %lu GB\n", (unsigned long) rlp.rlim_max / onegb);
  
  getrlimit (RLIMIT_FSIZE, &rlp);
  printf ("Soft limit on max file size     = %lu GB\n", (unsigned long) rlp.rlim_cur / onegb);
  printf ("Hard limit on max file size     = %lu GB\n", (unsigned long) rlp.rlim_max / onegb);
  
  getrlimit (RLIMIT_STACK, &rlp);
  printf ("Soft limit on max stack size    = %lu GB\n", (unsigned long) rlp.rlim_cur / onegb);
  printf ("Hard limit on max stack size    = %lu GB\n", (unsigned long) rlp.rlim_max / onegb);

#if ( defined SGI )
  getrlimit (RLIMIT_RSS, &rlp);
  printf ("Soft limit on max RSS           = %lu GB\n", (unsigned long) rlp.rlim_cur / onegb);
  printf ("Hard limit on max RSS           = %lu GB\n", (unsigned long) rlp.rlim_max / onegb);
  getrlimit (RLIMIT_VMEM, &rlp);
  printf ("Soft limit on max virtual memory= %lu GB\n", (unsigned long) rlp.rlim_cur / onegb);
  printf ("Hard limit on max virtual memory= %lu GB\n", (unsigned long) rlp.rlim_max / onegb);
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
    printf ("Max mallocable memory = %d GB within 10%%\n", mbytes / 1024);
  }

  exit(0);
}
