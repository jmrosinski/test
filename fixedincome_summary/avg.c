#include <stdio.h>

#define LINESIZE 128  // max size of input line

int main ()
{
  char line[LINESIZE]; // input line
  char infile[24];
  FILE *fp;
  float inflation;             // inflation (%)
  float amt, amttot;           // current value ($)
  float income, incometot;     // income
  float actual, actualtot;     // income after taxes
  float realized, realizedtot; // realized gain after taxes and inflation
  float rate;                  // interest rate (%) 
  float basis, basistot;       // cost basis
  float taxrate;               // aggregate tax rate (%)
  float wbar;
  int nument;

  printf ("Enter input file\n");
  scanf ("%23s", infile);

  // Open required input file
  if ((fp = fopen (infile, "r")) == NULL) {
    printf ("cannot open input file %s\n", infile);
    return 1;
  }
  printf ("Successfully opened infile=%s\n", infile);

  printf ("Enter inflation rate (percent)\n");
  scanf ("%f", &inflation);
  inflation *= 0.01;

  // Skip EXACTLY 1 LINE (header)
  fgets (line, LINESIZE, fp);
  nument      = 0;
  amttot      = 0.;
  incometot   = 0.;
  actualtot   = 0.;
  realizedtot = 0.;
  basistot    = 0.;
  // Parse input lines assuming entries are: amount rate basis taxrate comment
  while (fgets (line, LINESIZE, fp) != NULL) {
    if (sscanf (line, "%f %f %f %f %*s", &amt, &rate, &basis, &taxrate) != 4) {
      printf ("bad input line=%s\n", line);
      return 1;
    }
    rate    *= 0.01;
    taxrate *= 0.01;
    ++nument;
    income   = amt*rate;
    actual   = income*(1. - taxrate);
    realized = actual - inflation*amt;

    amttot      += amt;
    incometot   += income;
    actualtot   += actual;
    realizedtot += realized;
    basistot    += basis;
  }
  wbar = incometot / amttot;
  printf ("total fixed income assets=%f K\n", amttot);
  printf ("total fixed income basis= %f K\n", basistot);
  printf ("weighted rate for %d entries=%f percent\n", nument, 100.*wbar);
  printf ("yearly income=$%.2fK\n", incometot);
  printf ("yearly income after taxes taken out=$%.2fK\n", actualtot);
  printf ("yearly income after taxes and inflation taken out=$%.2fK\n", realizedtot);
  return 0;
}
