#include <stdio.h>

int main ()
{
  double startamt; // initial investment value
  double growth;   // yearly percentage average growth of investment
  double holdyrs;  // years to hold investment
  double taxrate;  // yearly tax rate (percent) assumed constant over time
  double rothamt;  // Roth value over time (taxed at start but not again)
  double atamt;    // Left in taxable account but taxed after holdyrs
  double frac;     // Fractional time in case holdyrs is not an integer
  double attax;    // tax on atamt at end of holdyrs
  double atfinal;  // atamt after tax applied at end of holdyrs
  
  printf ("Enter starting amount (thousands of $)\n");
  scanf ("%lf", &startamt);
  printf ("Enter estimated yearly gain (%%)\n");
  scanf ("%lf", &growth);
  printf ("Enter length to hold (floating point years)\n");
  scanf ("%lf", &holdyrs);
  printf ("Enter effective tax rate on income (%%)\n");
  scanf ("%lf", &taxrate);

  rothamt = startamt*(1. - taxrate*0.01);
  atamt   = startamt;
  for (int start = 0; start < (int) holdyrs; ++start) {
    rothamt += growth*0.01*rothamt;
    atamt   += growth*0.01*atamt;
  }
  frac = holdyrs - (int) holdyrs;
  if (frac > 0.) {
    rothamt += frac*growth*0.01*rothamt;
    atamt   += frac*growth*0.01*atamt;
  }
  attax   = taxrate*0.01*atamt;
  atfinal = atamt - attax;
  printf ("Results after holding $%.1lfK for %.1lf years:\n", startamt, holdyrs);
  printf ("rothamt=$%.2lfK\n", rothamt);
  printf ("taxes will decrease attamt=$%.2lfK by $%.2lfK to a value of $%.2lfK\n",
	  atamt, attax, atfinal);
  return 0;
}
