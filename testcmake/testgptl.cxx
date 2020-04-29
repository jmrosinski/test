#include <gptl.h>
#include <stdio.h>

int main()
{
  int ret;

  ret = GPTLinitialize ();
  ret = GPTLstart ("main");
  ret = GPTLstop ("main");
  ret = GPTLpr (0);
  return 0;
}
