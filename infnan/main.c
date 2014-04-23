#include <stdio.h>

typedef union {
  struct {
    unsigned int bit : 1;
  } bitarr[32];
  struct {
    unsigned char bit8;
  } uchararr[4];
  float x;
} Bits32;

typedef union {
  struct {
    unsigned int bit : 1;
  } bitarr[64];
  struct {
    unsigned char bit8;
  } uchararr[8];
  double x;
} Bits64;

extern void prbitpat32 (Bits32);
extern void prbitpat64 (Bits64);

int main (int argc, char **argv)
{
  const float zero32 = 0.;
  const float one32 = 1.;
  const double zero64 = 0.;
  const double one64 = 1.;

  Bits32 var32;
  Bits64 var64;

  printf ("sizeof unsigned char=%d\n", sizeof (unsigned char));
  var32.x = one32 / zero32;
  printf ("32-bit inf=%f\n", var32.x);
  printf ("bits of 32-bit inf:\n");
  prbitpat32 (var32);

  var64.x = one64 / zero64;
  printf ("64-bit inf=%f\n", var64.x);
  printf ("bits of 64-bit inf:\n");
  prbitpat64 (var64);

  var32.x = zero32 / zero32;
  printf ("32-bit nan=%f\n", var32.x);
  printf ("bits of 32-bit nan:\n");
  prbitpat32 (var32);

  var64.x = zero64 / zero64;
  printf ("64-bit nan=%f\n", var64.x);
  printf ("bits of 64-bit nan:\n");
  prbitpat64 (var64);

  return 0;
}

void prbitpat32 (Bits32 var32)
{
  int i;

  printf ("bits:\n");
  for (i = 0; i < 32; ++i)
    printf ("%1u", var32.bitarr[i].bit);
  printf ("\n");

  printf ("hex:\n");
  for (i = 0; i < 4; ++i)
    printf ("%x", var32.uchararr[i].bit8);
  printf ("\n");
}

void prbitpat64 (Bits64 var64)
{
  int i;

  printf ("bits:\n");
  for (i = 0; i < 64; ++i)
    printf ("%1u", var64.bitarr[i].bit);
  printf ("\n");

  printf ("hex:\n");
  for (i = 0; i < 8; ++i)
    printf ("%x", var64.uchararr[i].bit8);
  printf ("\n");
}
