#include <stdio.h>
int main ()
{
  const double waiting_percent = 8.; // ABOVE inflation yearly ss increase by waiting till age 70
  const double arb = 1.;             // yearly traunch for early start. Value is arbitrary as it
                                     //   has no impact when comparing to starting SS later
  double banked_fraction;            // above inflation percent banked when wait till age 70
  const int late_start_age = 70;     // latest age benefits can start
  int start_age = 62;                // age benefits start
  int age;                           // iterator for age of recipient
  double invest_percent;             // ABOVE inflation growth from investing 100% of ss income
  double early_total;                // accum wealth starting at age start_age
  double late_total;                 // accum wealth starting at age 70
  int done, foundit;                 // flags to exit loop

  printf ("Enter percent above inflation expected for investing soc sec income\n");
  scanf ("%lf", &invest_percent);
  do {
    done = 1;
    printf ("Enter age to start taking soc sec benefits\n");
    scanf ("%d", &start_age);
    if (start_age < 62 || start_age > 70) {
      printf ("start_age must be between 62 and 70\n");
      done = 0;
    }
  } while (! done);

  early_total     = 0.;
  late_total      = 0.;
  age             = start_age;
  banked_fraction = 1.;
  foundit         = 0;
  while (age < 100) {
    // *_total values are for the end of the year in question
    // "arb" is the early yearly traunch
    // "banked_fraction" is the initial traunch when waiting till latest possible age
    // 0.005 is due to accumulating new income monthly over an entire year and investing it.
    early_total += arb*(1. + invest_percent*0.005) +     // invested income this yr traunch
                   early_total*(invest_percent*0.01);    // invested income from prv yrs total
    if (age < late_start_age) {
      banked_fraction += banked_fraction*(waiting_percent*0.01);  // income increase by waiting
    } else {
       // invested income this yr traunch + invested income from prv yrs total
      late_total += arb*banked_fraction*(1. + invest_percent*0.005) +
	            late_total*(invest_percent*0.01);
    }
    if (late_total >= early_total) {
      foundit = 1;
      break;
    }
    ++age;
  }
  printf ("Results for scenario:\n");
  printf ("ss yearly percent increase for waiting beyond age 62=%.1lf\n", waiting_percent);
  printf ("age to start benefits=%d\n", start_age);
  printf ("Assumed yearly percent above inflation by investing ss benefits=%.1lf\n",
	  invest_percent);
  printf ("Early traunch=%.0lf late traunch=%.2lf\n", arb, banked_fraction);

  if (foundit)
    printf ("age=%d late_total=%lf first exceeds early_total=%lf\n", age, late_total, early_total);
  else
    printf ("age=%d late_total=%lf STILL does not exceed early_total=%lf\n",
	    age, late_total, early_total);
  return 0;
}
