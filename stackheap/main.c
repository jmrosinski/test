#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main (int argc, char **argv)
{
  int nthreads;
  int n;
  void *threadstack_startaddr[3];
  char *addr1, *addr2;

  void threadedsub (int, void **);
  void recursivesub (int, void *);

  omp_set_num_threads (3);
  nthreads = omp_get_max_threads ();

#pragma omp parallel for
  for (n = 0; n < nthreads; ++n) {
    threadedsub (n, threadstack_startaddr);
  }

  printf ("threadstack[1] starts at %p\n"
	  "threadstack[2] starts at %p\n"
	  "delta=%d\n", 
	  threadstack_startaddr[1],
	  threadstack_startaddr[2],
	  (int) (threadstack_startaddr[2] - threadstack_startaddr[1]));

  printf ("stack starts at %p which is %d from threadstack at %p\n", 
	  &nthreads, (int) (threadstack_startaddr[1] - (void *) &nthreads), threadstack_startaddr[1]);

  recursivesub (2, addr1);
  addr1 = (char *) malloc (1000);
  addr2 = (char *) malloc (1000);
  
  printf ("heap addr2 - heap addr1 1k bytes apart=%d\n", (int) (addr2 - addr1));
  return 0;
}

/*
** Report back to caller the address of where my local stack variables (or more likely threadstack) start
*/
void threadedsub (int n, void *threadstackaddr[3])
{
  unsigned long var;

  printf ("threadsub: writing %p into threadstackaddr\n", &var);
  threadstackaddr[n] = (void *) &var;
}

/*
** 3 levels of recursion: 
**   level 2 invokes level1, passing where his threadstack array begins
**   level 1 prints how far away his stack starts from parent (level 2), then invokes level 0
**   level 0 just returns
*/
void recursivesub (int n, void *parentarraddr)
{
  char arr[1000];

  if (n == 0) {
    return;
  } else if (n < 2) {
    printf ("recursivesub: address of 1k arr starts at %p, %d bytes from parentarr which starts at %p\n",
	    &arr[0], (int) (parentarraddr - (void *) &arr[0]), parentarraddr);
  }
  recursivesub (n-1, &arr[0]);
}
