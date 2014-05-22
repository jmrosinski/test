// Need to define _GNU_SOURCE sometimes to get value of CPU_SETSIZE
#define _GNU_SOURCE

#include <stdio.h>
#include <sched.h>

void runnable (cpu_set_t *mask, int *lo_out, int *hi_out)
{
  int lo;
  int hi = -1;

  // Loop through core set, find first range of runnable cores and store results in "lo" and "hi"
  for (lo = 0; lo < CPU_SETSIZE; ++lo) {
    if (CPU_ISSET (lo, mask)) {
      for (hi = lo+1; hi < CPU_SETSIZE; hi++) { 
	if ( ! CPU_ISSET (hi, mask))
	  break;
      }
      --hi;         // Change index from first no to last yes
      break;
    }
  }

  // Set return values
  *lo_out = lo;
  *hi_out = hi;

  // Check to see if another range exists
  if (hi < CPU_SETSIZE) {
    for (lo = hi+1; lo < CPU_SETSIZE; ++lo) {
      if (CPU_ISSET (lo, mask)) {
	break;
      }
    }
    if (lo < CPU_SETSIZE) {
      printf ("runnable: found another runnable range starting at core=%d. lo_out=%d hi_out=%d\n",
	      lo, *lo_out, *hi_out);
    }
  }
}
