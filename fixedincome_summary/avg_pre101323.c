#include <stdio.h>

#define LINESIZE 128  // max size of input line

int main ()
{
  char line[LINESIZE];   // input line
  char infile[24];
  FILE *fp;
  float rate;
  float amt, amttot;
  float wixitot;
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

  // Skip EXACTLY 1 LINE (header)
  fgets (line, LINESIZE, fp);
  nument = 0;
  amttot = 0.;
  wixitot = 0.;
  // Parse input lines assuming entries are: amount rate comment
  while (fgets (line, LINESIZE, fp) != NULL) {
    if (sscanf (line, "%f %f %*s", &amt, &rate) != 2) {
      printf ("bad input line=%s\n", line);
      return 1;
    }
    ++nument;
    amttot += amt;
    wixitot += amt*rate;
  }
  wbar = wixitot / amttot;
  printf ("total fixed income=%f K\n", amttot);
  printf ("average rate for %d entries=%f percent\n", nument, wbar);
  printf ("yearly income=$%.2fK\n", 0.01*wbar*amttot);
  return 0;
}
