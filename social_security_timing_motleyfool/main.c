#include <stdio.h>

struct FRA {
  int yr;
  int mo;
};

void get_start (struct FRA *, const char *);
double alter_benefits (const struct FRA, const struct FRA);

// Find break-even age for taking social security benefits early vs. late
// Inputs:
//   birth date (full retirement age depends on this)
//   early starting age, late starting age (must be between 62 and 70)
//   assumed above-inflation yearly percentage achieved by investing ss income
// Output:
//   age at which starting late total benefits first exceed starting early benefits, assuming
//   all benefits are invested and earn specified yearly %
//
// Source of adjustments to ss benefits:
//   www.fool.com/investing/how-much-does-waiting-claim-raise-social-security.aspx

int main ()
{
  int birth_yr;                  // birth year
  struct FRA fra;                // full retirement age
  struct FRA early_start;        // early age to start benefits
  struct FRA late_start;         // late age to start benefits for comparison
  double early_fraction;         // early tranch adjusted from full retirement age
  double late_fraction;          // late tranch adjusted from full retirement age
  double invest_percent;         // ABOVE inflation growth from investing 100% of ss income
  double early_total;            // accum wealth starting at age early_start
  double late_total;             // accum wealth starting later than early_start
  int done, foundit;             // flag to exit loop
  int yr, mo;                    // year and month iterators

  printf ("Enter birth year but NOTE: if born Jan 1 enter previous year\n");
  scanf ("%d", &birth_yr);
  if (birth_yr < 1943) {
    printf ("Behavior for birth years earlier than 1943 is unknown. Giving up\n");
    return 1;
  } else if (birth_yr < 1955) {
    fra.yr = 66;
    fra.mo = 1;
  } else if (birth_yr > 1959) {
    fra.yr = 67;
    fra.mo = 1;
  } else {
    switch (birth_yr) {
    case 1955: fra.yr = 66; fra.mo = 2; break;
    case 1956: fra.yr = 66; fra.mo = 4; break;
    case 1957: fra.yr = 66; fra.mo = 6; break;
    case 1958: fra.yr = 66; fra.mo = 8; break;
    case 1959: fra.yr = 66; fra.mo = 10; break;
    }
  }
  printf ("born in %d gives fra=%d %d\n", birth_yr, fra.yr, fra.mo);

  printf ("Enter percent above inflation expected for investing soc sec income\n");
  scanf ("%lf", &invest_percent);

  do {
    done = 1;
    get_start (&early_start, "early");
    get_start (&late_start, "late");
    if (late_start.yr < early_start.yr ||
      late_start.yr == early_start.yr && late_start.mo < early_start.mo) {
      printf (" early start age=%d %d cannot exceed late start age=%d %d\n",
	      early_start.yr, early_start.mo, late_start.yr, late_start.mo);
      done = 0;
    }
  } while (! done);

  early_fraction = alter_benefits (fra, early_start);
  late_fraction = alter_benefits (fra, late_start);
  printf ("Early benefits will be changed to %.2lf of full retirement age\n", early_fraction);
  printf ("Late benefits will be changed to %.2lf of full retirement age\n", late_fraction);
  
  early_total    = 0.;
  late_total     = 0.;
  foundit        = 0;
  for (yr = early_start.yr; yr < 100; ++yr) {
    mo = yr == early_start.yr ? early_start.mo : 1;
    while (mo <= 12) {
      // *_total values are for the end of the month and year in question
      // divide by 1200 converts yearly percent to monthly fraction
      //   If per-year ss were doled out entirely on Jan 1 this number would be 0.01 (% to fraction)
      early_total += early_fraction*(1. + invest_percent/1200.) + // income this mo tranch
	             early_total*(invest_percent/1200.);          // income from prv total
      if (yr == late_start.yr && mo >= late_start.mo ||
	  yr > late_start.yr) {
	late_total += late_fraction*(1. + invest_percent/1200.) + // income this mo tranch
	              late_total*(invest_percent/1200.);          // income this prv total
      }
      if (late_total >= early_total) {
	foundit = 1;
	goto print_results;            // exit 2 nested loops
      }
      ++mo;
    }
  }
  print_results:
  printf ("Results for scenario:\n");
  printf ("Early yr mo to start benefits=%d %d\n", early_start.yr, early_start.mo);
  printf ("Late yr mo to start benefits=%d %d\n", late_start.yr, late_start.mo);
  printf ("Assumed yearly percent above inflation by investing ss benefits=%.1lf\n",
	  invest_percent);
  printf ("Early tranch=%.2lf late tranch=%.2lf\n", early_fraction, late_fraction);

  if (foundit)
    printf ("age=%d %d late_total=%lf first exceeds early_total=%lf\n",
	    yr, mo, late_total, early_total);
  else
    printf ("age=%d %d late_total=%lf STILL does not exceed early_total=%lf\n",
	    yr, mo, late_total, early_total);
  return 0;
}

void get_start (struct FRA *start, const char *txt)
{
  int done = 1;
  do {
    printf ("Enter %s age to start taking ss income as year followed by month\n", txt);
    scanf ("%d %d", &start->yr, &start->mo);
    if (start->yr < 62 || start->yr > 70) {
      printf (" start year must be between 62 and 70\n");
      done = 0;
    }
    if (start->mo < 1 || start->mo > 12 ||
	(start->yr == 70 && start->mo != 1)) {
      printf (" start month must be between 1 and 12, or =1 to start at age 70\n");
      done = 0;
    }
  } while (! done);
}

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

double alter_benefits (const struct FRA fra, const struct FRA start)
{
  double fraction;  // return value: positive means starting later than FRA, negative earlier
  int nmo;          // number of months varied from fra

  fraction = 1.;    // Value for start = fra
  nmo = (start.yr - fra.yr)*12 + (start.mo - fra.mo);
  if (nmo > 0) {
    // benefits will be increased. 0.01 converts % to fraction
    fraction += (0.01*2./3.) * (double) nmo;
  } else {
    // benefits will be reduced. 0.01 converts % to fraction
    nmo = -nmo;
    fraction -= (0.01*5./9.) * (double) MIN (36, nmo);
    if (nmo > 36)
      fraction -= (0.01*5./12.) * (double) (nmo - 36);
  }
  return fraction;
}
