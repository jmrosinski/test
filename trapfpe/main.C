#include <fenv.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#ifdef __APPLE__
#include <xmmintrin.h>
#endif

extern "C" {
  float fortfunc_ (float *, float *);    // Fortran function generates FPE
  int ifortfunc_div_ (int *, int *);   // Fortran function generates FPE
  int ifortfunc_mult_ (int *, int *);  // Fortran function generates FPE

  float cfunc_ (float *, float *);       // C function generates FPE
  int icfunc_div_ (int *, int *);      // Fortran function generates FPE
  int icfunc_mult_ (int *, int *);     // Fortran function generates FPE
}

int main ()
{
  float z;
  int iz;
  int ione = 1;
  int izero = 0;
  int ibig = 2000000000;
  float one = 1.F;
  float zero = 0.F;
  float small = 1.e-30F;
  float big = 1.e+30F;
  int ans;
  char lang[2];
  int ret;
  struct sigaction sig_action = {};
  extern void handler (int, siginfo_t *, void *);                 // Signal handler

#ifdef __APPLE__
  _MM_SET_EXCEPTION_MASK(_MM_GET_EXCEPTION_MASK() & ~_MM_MASK_INVALID);
  _MM_SET_EXCEPTION_MASK(_MM_GET_EXCEPTION_MASK() & ~_MM_MASK_DIV_ZERO);
  _MM_SET_EXCEPTION_MASK(_MM_GET_EXCEPTION_MASK() & ~_MM_MASK_OVERFLOW);
#else
  ret = feenableexcept (FE_DIVBYZERO | FE_INVALID | FE_OVERFLOW);
#endif

  sig_action.sa_sigaction = handler;
  sigemptyset (&sig_action.sa_mask);
  sig_action.sa_flags = SA_SIGINFO | SA_ONSTACK;

  if (sigaction (SIGFPE,  &sig_action, NULL) != 0) {
    printf ("Call to sigaction failed: %s\n", strerror (errno));
    return -1;
  }

  while (1) {
    printf ("Enter 0 for divide by zero, 1 for overflow, 2 for underflow, 3 for invalid:\n");
    printf ("Enter 4 for integer divide by zero, 5 for integer overflow:\n");
    scanf ("%d", &ans);

    printf ("Enter f for Fortran computational routine, c for C:\n");
    scanf ("%s", lang);

    if (lang[0] == 'c') {
      switch (ans) {
      case 0:
	printf ("Callign cfunc to divide by zero (1./0.):\n");
	z = cfunc_ (&one, &zero);
	printf ("result=%f\n", z);
	break;
      case 1:
	printf ("Calling cfunc to overflow (big/small):\n");
	z = cfunc_ (&big, &small);
	printf ("result=%f\n", z);
	break;
      case 2:
	printf ("Calling cfunc to underflow (small/big):\n");
	z = cfunc_ (&small, &big);
	printf ("result=%f\n", z);
	break;
      case 3:
	printf ("Calling cfunc to issue invalid instruction (0./0.):\n");
	z = cfunc_ (&zero, &zero);
	printf ("result=%f\n", z);
	break;
      case 4:
	printf ("Calling icfunc to issue integer divide by zero (1/0):\n");
	iz = icfunc_div_ (&ione, &izero);
	printf ("result=%d\n", iz);
	break;
      case 5:
	printf ("Calling icfunc to issue integer overflow (ibig * ibig):\n");
	iz = icfunc_mult_ (&ibig, &ibig);
	printf ("result=%d\n", iz);
	break;
      default:
	printf ("You must enter 0, 1, 2, or 3: %d is not valid\n", ans);
      }
    } else if (lang[0] == 'f') {
      switch (ans) {
      case 0:
	printf ("Callign fortfunc to divide by zero (1./0.):\n");
	z = fortfunc_ (&one, &zero);
	printf ("result=%f\n", z);
	break;
      case 1:
	printf ("Calling fortfunc to overflow (big/small):\n");
	z = fortfunc_ (&big, &small);
	printf ("result=%f\n", z);
	break;
      case 2:
	printf ("Calling fortfunc to underflow (small/big):\n");
	z = fortfunc_ (&small, &big);
	printf ("result=%f\n", z);
	break;
      case 3:
	printf ("Calling fortfunc to issue invalid instruction (0./0.):\n");
	z = fortfunc_ (&zero, &zero);
	printf ("result=%f\n", z);
	break;
      case 4:
	printf ("Calling ifortfunc_div to issue integer divide by zero (1/0):\n");
	iz = ifortfunc_div_ (&ione, &izero);
	printf ("result=%d\n", iz);
	break;
      case 5:
	printf ("Calling ifortfunc_mult to issue integer overflow (ibig * ibig):\n");
	iz = ifortfunc_mult_ (&ibig, &ibig);
	printf ("result=%d\n", iz);
	break;
      default:
	printf ("You must enter 0, 1, 2, or 3: %d is not valid\n", ans);
      }
    }
  }
}

