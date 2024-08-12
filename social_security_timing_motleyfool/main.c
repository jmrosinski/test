#include <stdio.h>

struct AGE {
  int yrs;
  int mos;
};

void get_start (struct AGE *, const char *);
double alter_benefits (const struct AGE, const struct AGE);

// Find break-even age for starting social security benefits at 2 different times
// Inputs:
//   birth date (full retirement age depends on this)
//   early starting age, late starting age (must be between 62 and 70)
//   assumed above-inflation yearly percentage achieved by investing ss income
// Output:
//   age at which starting benefits later first exceeds starting benefits earlier, assuming
//   all benefits are invested and earn user-specified yearly % (can even be 0 or negative)
//
// Source of adjustments to ss benefits based on birth date and age benefits are started:
//   www.fool.com/investing/how-much-does-waiting-claim-raise-social-security.aspx

int main ()
{
  struct DATE {
    int yr;
    int mo;
  } bdate;                       // birth date
  struct AGE fra;                // full retirement age
  struct AGE early_start;        // early age to start benefits
  struct AGE late_start;         // late age to start benefits for comparison
  double early_fraction;         // early tranch adjusted from full retirement age
  double late_fraction;          // late tranch adjusted from full retirement age
  double invest_percent;         // ABOVE inflation growth from investing 100% of ss income
  double early_total;            // accum wealth starting at age early_start
  double late_total;             // accum wealth starting later than early_start
  int done, foundit;             // flag to exit loop
  int yrs, mos;                  // years and months iterators

  printf ("Enter birth date as \"year mo\" but NOTE:\n");
  printf ("If born 1st of month enter previuos month (and year if applicable) as birth date\n");
  scanf ("%d %d", &bdate.yr, &bdate.mo);
  if (bdate.yr < 1943) {
    printf ("Behavior for birth years earlier than 1943 is unknown. Giving up\n");
    return 1;
  } else if (bdate.yr < 1955) {
    fra.yrs = 66;
    fra.mos = 0;
  } else if (bdate.yr > 1959) {
    fra.yrs = 67;
    fra.mos = 0;
  } else {
    fra.yrs = 66;
    switch (bdate.yr) {
    case 1955: fra.mos = 2; break;
    case 1956: fra.mos = 4; break;
    case 1957: fra.mos = 6; break;
    case 1958: fra.mos = 8; break;
    case 1959: fra.mos = 10; break;
    default:
      printf ("Unknown bdate.yr=%d\n", bdate.yr);
      return 2;
    }
  }
  printf ("birth yr=%d mo=%d gives full retirement age=%d yrs %d mos\n",
	  bdate.yr, bdate.mo, fra.yrs, fra.mos);

  printf ("Enter percent above inflation expected for investing soc sec income\n");
  scanf ("%lf", &invest_percent);

  do {
    done = 1;
    get_start (&early_start, "early");
    get_start (&late_start, "late");
    if (late_start.yrs < early_start.yrs ||
      late_start.yrs == early_start.yrs && late_start.mos < early_start.mos) {
      printf (" early start age=%d yrs %d mos cannot exceed late start age=%d yrs %d mos\n",
	      early_start.yrs, early_start.mos, late_start.yrs, late_start.mos);
      done = 0;
    }
  } while (! done);

  early_fraction = alter_benefits (fra, early_start);
  late_fraction = alter_benefits (fra, late_start);
  
  early_total    = 0.;
  late_total     = 0.;
  foundit        = 0;
  for (yrs = early_start.yrs; yrs < 100; ++yrs) {
    mos = yrs == early_start.yrs ? early_start.mos : 0;
    while (mos < 12) {
      // *_total values are for the end of the month and year in question
      // divide by 1200 converts yearly percent to monthly fraction
      //   If per-year ss were doled out entirely on Jan 1 this number would be 0.01 (% to fraction)
      early_total += early_fraction*(1. + invest_percent/1200.) + // income from this mo tranch
	             early_total*(invest_percent/1200.);          // income from prv total
      if (yrs == late_start.yrs && mos >= late_start.mos ||
	  yrs > late_start.yrs) {
	late_total += late_fraction*(1. + invest_percent/1200.) + // income from this mo tranch
	              late_total*(invest_percent/1200.);          // income from prv total
      }
      if (late_total >= early_total) {
	foundit = 1;
	goto print_results;            // exit 2 nested loops
      }
      ++mos;
    }
  }
  print_results:
  printf ("Results for scenario:\n");
  printf ("Early age to start benefits=%d yrs %d mos\n", early_start.yrs, early_start.mos);
  printf ("Late age to start benefits=%d yrs %d mos\n", late_start.yrs, late_start.mos);
  printf ("Assumed yearly percent above inflation by investing ss benefits=%.1lf\n",
	  invest_percent);
  printf ("Early tranch=%.2lf late tranch=%.2lf\n", early_fraction, late_fraction);

  if (foundit)
    printf ("age=%d %d late_total=%lf first exceeds early_total=%lf\n",
	    yrs, mos, late_total, early_total);
  else
    printf ("age=%d %d late_total=%lf STILL does not exceed early_total=%lf\n",
	    yrs, mos, late_total, early_total);
  return 0;
}

void get_start (struct AGE *start, const char *txt)
{
  int done = 1;
  do {
    printf ("Enter %s age to start taking ss income as years plus months\n", txt);
    scanf ("%d %d", &start->yrs, &start->mos);
    if (start->yrs < 62 || start->yrs > 70) {
      printf (" start year must be between 62 and 70\n");
      done = 0;
    }
    if (start->mos < 0 || start->mos > 11 || (start->yrs == 70 && start->mos != 0)) {
      printf (" start months must be between 0 and 11, or =0 to start at age 70\n");
      done = 0;
    }
  } while (! done);
}

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

// alter_benefits()
// apply adjustments to monthly ss benefits based on formulas from motley fool
// Inputs:
//   fra: full retirement age (years months)
//   start: age to start benefits (years months)
// Return value: fraction of full retirement benefits to be received.
//   If start < fra, fraction will be < 1. If start > fra, fraction will be > 1.
double alter_benefits (const struct AGE fra, const struct AGE start)
{
  double fraction;  // return value: <1 means starting earlier than fra, >1 means later
  int nmo;          // number of months varied from fra

  fraction = 1.;    // Value for start = fra
  nmo = (start.yrs - fra.yrs)*12 + (start.mos - fra.mos);
  if (nmo > 0) {
    // benefits will be increased from fra due to earlier start. 0.01 converts % to fraction
    // equation from motley fool
    fraction += (0.01*2./3.) * (double) nmo;
  } else if (nmo < 0) {
    // benefits will be reduced from fra due to later start. 0.01 converts % to fraction
    // equation from motley fool
    nmo = -nmo;
    fraction -= (0.01*5./9.) * (double) MIN (36, nmo);
    if (nmo > 36)
      fraction -= (0.01*5./12.) * (double) (nmo - 36);
  }
  return fraction;
}
