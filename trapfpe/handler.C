#include <execinfo.h>  // backtrace*
#include <stdio.h>     // printf
#include <stdlib.h>    // abort
#include <unistd.h>    // stderr
#include <signal.h>    // siginfo_t

static const int maxfuncs = 10;

void handler (int sig, siginfo_t *info, void *ucontext) {
  void *stack[maxfuncs];
  size_t nfuncs;

  fprintf(stderr, "Caught SIGFPE: ");

  switch (info->si_code) {
  case FPE_INTDIV:
    fprintf (stderr, "integer divide by zero\n");
    break;
  case FPE_INTOVF:
    fprintf (stderr, "integer overflow)\n");
    break;
  case FPE_FLTDIV:
    fprintf (stderr, "floating-point divide by zero\n");
    break;
  case FPE_FLTOVF:
    fprintf (stderr, "floating-point overflow\n");
    break;
  case FPE_FLTUND:
    fprintf (stderr, "floating-point underflow\n");
    break;
  case FPE_FLTRES:
    fprintf (stderr, "floating-point inexact result\n");
    break;
  case FPE_FLTINV:
    fprintf (stderr, "floating-point invalid operation\n");
    break;
  case FPE_FLTSUB:
    fprintf (stderr, "subscript out of range\n");
    break;
  default:
    fprintf (stderr, "Arithmetic Exception\n");
    break;
  }

  nfuncs = backtrace (stack, maxfuncs);
  backtrace_symbols_fd (&stack[0], nfuncs, STDERR_FILENO);
  abort ();
}
