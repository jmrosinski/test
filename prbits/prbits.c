#include <stdio.h>
#include <stdint.h>

void prbits4_ (uint32_t *val)
{
  uint32_t mask = 1;
  uint32_t locval = *val;
  char chars[32];
  
  int i;

  printf ("bits: ");
  for (i = 0; i < 32; ++i) {
    if ((locval & mask) == 0) 
      chars[i] = '0';
    else
      chars[i] = '1';
    locval >>= 1;
  }
  for (i = 0; i < 32; ++i)
    printf ("%c", chars[31-i]);
  printf ("\n");
}

void prbits8_ (uint64_t *val)
{
  uint64_t mask = 1;
  uint64_t locval = *val;
  char chars[64];
  
  int i;

  printf ("bits: ");
  for (i = 0; i < 64; ++i) {
    if ((locval & mask) == 0) 
      chars[i] = '0';
    else
      chars[i] = '1';
    locval >>= 1;
  }
  for (i = 0; i < 64; ++i)
    printf ("%c", chars[63-i]);
  printf ("\n");
}
