#include "device.h"
#include <stdio.h>

int main () {
  const int smcount = 5;
  const int warpsize = 32;
  const int warps_per_sm = 4;
  int ret;

  printf ("starting main, sizeof address=%ld\n", sizeof (int *));
#pragma acc parallel loop private(ret)
  for (int sm = 0; sm < smcount; ++sm) {
#pragma acc loop worker
    for (int w = 0; w < warps_per_sm; ++w) {
#pragma acc loop vector
      for (int i = 0; i < warpsize; ++i) {
	ret = whoami ();
      }
    }
  }
  return 0;
}
