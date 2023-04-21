#include <stdio.h>
#include <gptl.h>

int main()
{
  int ret;
  int zero = 0;
  
  printf ("Entered cputest\n");
  ret = GPTLstart (zero);
  ret = GPTLstop (zero);
  return 0;
}
