#include <stdio.h>
#include <stdbool.h>
int main (int argc, const char **argv)
{
  double early_ttl, late_ttl;       // inflation-adjusted total income
  double early_income, late_income; // income values from TIAA, adjusts over time
  double inflation;                 // read in as percentage, change to fraction
  double assets;                    // starting assets (early)
  double late_assets;               // late assets (increased vs early due to waiting)
  double growth_rate;               // growth rate for late assets before annuitizing
  int nyrs;                         // number of years between early and late start
  bool done = false;                // loop terminates when done is true
  bool doprint1 = true;
  bool doprint2 = true;
  
  do {
    printf ("Enter nyrs (number of years difference between early and late)\n");
    scanf ("%d", &nyrs);
    printf ("Enter early_income late_income in thousands of $\n");
    while (scanf ("%lf %lf", &early_income, &late_income) != 2);
  } while (late_income < early_income);

  printf ("Enter fixed TIAA assets in thousands of $\n");
  scanf ("%lf", &assets);

  printf ("Enter asset growth rate as floating point percentage\n");
  scanf ("%lf", &growth_rate);
  growth_rate *= 0.01;

  printf ("Enter aggregate inflation rate as floating point percentage\n");
  scanf ("%lf", &inflation);
  inflation *= 0.01;

  late_assets = assets;
  for (int n = 0; n < nyrs; ++n)
    late_assets *= (1. + growth_rate - inflation);
  printf ("assets=%.3lfK\n", assets);
  printf ("late_assets=%.3lfK after adding %d years growth before starting\n", late_assets, nyrs);

  early_ttl = late_ttl = 0.;
  // loop over years until late_ttl exceeds early_ttl OR 30 years have passed
  for (int yr = 1; early_ttl >= late_ttl; ++yr) {
    if (yr > 1) { // Assume first year TIAA income estimates match year 1 so no inflation effect
      early_income *= (1. - inflation);
      late_income  *= (1. - inflation); // Assume initial late_income yr matches early_income yr
    }
    early_ttl    += early_income;
    if (yr > nyrs)
      late_ttl += late_income;

    printf ("yr=%d early_ttl=%.3lfK late_ttl=%.3lfK\n", yr, early_ttl, late_ttl);
    if (late_ttl > early_ttl) {
      printf ("yr=%d late_ttl=%.3lfK > early_ttl=%.3lfK\n", yr, late_ttl, early_ttl);
      done = true;
    }
    if (late_ttl > late_assets && doprint1) {
      printf ("NOTE yr=%d late_ttl=%.3lfK > late_assets=%.3lfK\n", yr, late_ttl, late_assets);
      printf ("  NOTE early_ttl=%.3lfK\n", early_ttl);
      doprint1 = false;
    }
    if (early_ttl > assets && doprint2) {
      printf ("NOTE yr=%d early_ttl=%.3lfK > assets=%.3lfK\n", yr, early_ttl, assets);
      printf ("  NOTE late_ttl=%.3lfK\n", late_ttl);
      doprint2 = false;
    }
    if (done) {
      printf ("  NOTE early_income=%.3lfK late_income=%.3lfK\n", early_income, late_income);
      return 0;
    }
  }
}
