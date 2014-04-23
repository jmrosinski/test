#include <stdio.h>
#include <stdint.h>

#define BITS 0b01111111100000000000000000000000

int main ()
{
  // Binary = 0b, hex=0x, octal=0
  typedef union {
    uint32_t ival32;
    float val32;
  } Both;

  Both both;
  both.ival32 = BITS;

  printf ("val=%f\n", both.val32);
  return 0;
}
