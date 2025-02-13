#include <stdio.h>

int main ()
{
  double startamt;    // initial investment value from traditional IRA or 401K
  double growth;      // yearly average percentage growth of investment
  double holdyrs;     // years to hold investment
  double taxrate;     // yearly tax rate (percent). Assumed constant over time
  double init_tax;    // taxes paid on initial conversion (Roth or aftertax)
  double roth_now;    // Convert IRA or 401K immediately to Roth
  double aftax_now;   // Convert IRA or 401K immediately to after tax acct
  double aftax_later; // Convert IRA or 401K to after tax AFTER holdyrs
  double basis;       // basis for aftax_later (only growth taxed after holdyrs)
  double frac;        // Fractional years in case holdyrs is not an integer
  double now_tax;     // tax on aftax_now after holdyrs
  double now_final;   // final value of aftax_now after all taxes
  double later_tax;   // tax when converting IRA at end of holdyrs
  double later_final; // final value of aftax_later after all taxes
  
  printf ("Enter starting amount (thousands of $)\n");
  scanf ("%lf", &startamt);
  printf ("Enter estimated yearly gain (%%)\n");
  scanf ("%lf", &growth);
  printf ("Enter length to hold (floating point years)\n");
  scanf ("%lf", &holdyrs);
  printf ("Enter effective tax rate on income (%%)\n");
  scanf ("%lf", &taxrate);
  taxrate *= 0.01;  // convert % to fraction
  growth  *= 0.01;  // convert % to fraction

  // all methods require paying taxes at some point. Only aftax_later avoids it initially
  // Only Roth avoids at end of holdyrs
  init_tax    = startamt*taxrate;
  roth_now    = startamt - init_tax;
  aftax_now   = startamt - init_tax;
  aftax_later = startamt;

  basis = aftax_now;                // needed later for tax calculation

  for (int start = 0; start < (int) holdyrs; ++start) {
    roth_now    += growth*roth_now;
    aftax_now   += growth*aftax_now;
    aftax_later += growth*aftax_later;
  }
  frac = holdyrs - (int) holdyrs;
  if (frac > 0.) {
    roth_now    += frac*growth*roth_now;
    aftax_now   += frac*growth*aftax_now;
    aftax_later += frac*growth*aftax_later;
  }
  now_tax   = (aftax_now - basis)*taxrate;  // tax only growth
  now_final = aftax_now - now_tax;
  
  later_tax   = aftax_later*taxrate;        // tax entirety upon conversion
  later_final = aftax_later - later_tax;
    
  printf ("Converting tradIRA $%.1lfK via 3 methods for %.1lf years:\n", startamt, holdyrs);
  printf ("roth_now=$%.2lfK Initial reduction (taxes)=$%.2lfK\n", roth_now, init_tax);
  printf ("now_final=$%.2lfK  Initial reduction (taxes)=$%.2lfK Final reduction (taxes)=$%.2lfK\n",
	  now_final, init_tax, now_tax);
  printf ("later_final=$%.2lfK  Final reduction (taxes)=$%.2lfK\n", later_final, later_tax);
  
  return 0;
}
