#include <stdio.h>
#include <assert.h>

int main ()
{
  int iam;
  int ntask;
  int step;
  int incr;
  int sendto;
  int recvfm;
  int dosend;
  int dorecv;
  int twoincr;

  while (1) {
    printf ("Enter ntask\n");
    scanf ("%d", &ntask);
    printf ("ntask=%d:\n", ntask);
    step = 1;
    for (incr = 1; incr < ntask; incr = twoincr) {
      twoincr = 2*incr;
      printf ("step=%d\n", step);
      for (iam = 0; iam < ntask; ++iam) {
	sendto = iam - incr;
	recvfm = iam + incr;
	dorecv = ((iam + twoincr) % twoincr == 0) && (recvfm < ntask);
	dosend = ((iam + incr) % twoincr == 0) && (sendto > -1);
	if (dosend) {
	  printf (" iam=%d sending to %d\n", iam, sendto);
	  assert (! dorecv);
	}
	if (dorecv) {
	  printf (" iam=%d recving fm %d\n", iam, recvfm);
	  assert (! dosend);
	}
      }
      ++step;
    }
  }
  return 0;
}
