#include "./gptl.h"
#include <string.h>

#define gptlstart gptlstart_
#define gptlstop gptlstop_

// Local function prototypes: Everything callable by Fortran requires C linkage
#ifdef __cplusplus
extern "C" {
#endif

int gptlstart (int *);
int gptlstop (int *);

int gptlstart (int *arg)
{
  return GPTLstart (*arg);
}

int gptlstop (int *arg)
{
  return GPTLstop (*arg);
}

#ifdef __cplusplus
}
#endif
