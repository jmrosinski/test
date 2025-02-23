#include <stdio.h>
#include <stdlib.h>  // malloc, free
#include <ctype.h>   // atoi
#include <string.h>  // memset and string functions
#include <math.h>

// To run: e.g. ./predict
// User input asked: year to examine, estimated death year, month
// Data file read is "yearly_data"
// Output: annualized inflation
//         % change in assets (accounting for $ spent that year)
//         $ can take out to ramp to 0 at death date
// NOTE: program gives up if any entries in file are "?" or year not found

#define LINESIZE 128  // max size of input line

char *gettoken (const char *);                  // create null-terminated token for textual entry
int get9tokens (char *, char **);               // parse all tokens from line
int contains_question (const char *);           // see if input contains "?"
double inflation_effects (double, double, int); // effect of inflation over time on annuity income

int main ()
{
  char line[LINESIZE];     // input line
  char year[5];            // year parsed from line
  int iyear;               // integer version of year
  int ret;
  FILE *fp;
  double inflation;        // calculated inflation rate (%)
  int months;              // number of months for calculation (normally 12)
  int havecpi;             // flag indicating both start_cpi and end_cpi were specified

  // Next 9 are parsed from input file
  int beg_mo;              // month of year to start calculations
  double beg_cpi=0;        // CPI for PREIOUS month to beg_mo
  int beg_assets;          // assets (thousands) at start of beg_mo
  int end_mo;              // month of year to end calculations
  double end_cpi=0;        // CPI for end_mo
  int end_assets;          // assets (thousands) at end of end_mo
  double spent;            // thousands spent from beg_mo through end_mo
  double annuity;          // monthly annuity income assumed fixed over time (started UCAR 9/2023)
  double socsec;           // monthly social security (assumed adjusted for inflation over time)
  const double taxrate = 0.18;  // assumed fed+state tax rate(frac) on  money spent
  double pretax;           // spent money with taxrate included

  double annuity_final;    // monthly annuity at end of prediction period (adjusted for inflation)
  int ramp_year;           // year to ramp to 0
  int ramp_mo;             // month to ramp to 0
  double change;           // change in asset value
  int nmonths;             // number of months to ramp to zero
  double percent;
  double permo_init;       // beg amount can withdraw per month
  double permo_final;      // end amount can withdraw per month (less than init due to inflation)
  double peryear_init;     // beg amount can withdraw per year
  double peryear_final;    // end amount can withdraw per year (less than init due to inflation)
  double future_inflation; // anticipated inflation (percent, user input) applies to fixed annuity
  char *tokens[9];         // useful data from line

  // Read through entries, skipping those that don't start with a digit
  printf ("Enter input year\n");
  scanf ("%4s", year);
  printf ("Enter end year and month for prediction ramp\n");
  scanf ("%d %d", &ramp_year, &ramp_mo);
  printf ("Enter expected average inflation over ramp period (floating percent)\n");
  scanf ("%lf", &future_inflation);

  // Open required input file
  if ((fp = fopen ("./yearly_data", "r")) == NULL) {
    printf ("cannot open input file yearly_data\n");
    return 1;
  }

  // Read through input file until desired year is read
  do {
    memset (line, 0, LINESIZE);
    if (fgets (line, LINESIZE, fp) == NULL) {
      printf ("fgets reached EOF before finding year=%s\n", year);
      return 1;
    }
  } while (strncmp (line, year, 4) != 0);

  if (get9tokens (line, tokens) < 0) {
    printf ("get9tokens failed\n");
    return 1;
  }

  havecpi = 1;
  beg_mo = atoi (tokens[0]);
  if (*tokens[1] == '?')
    havecpi = 0;
  else
    beg_cpi = atof (tokens[1]);

  beg_assets = atoi (tokens[2]);
  end_mo     = atoi (tokens[3]);
  if (*tokens[4] == '?')
    havecpi = 0;
  else
    end_cpi  = atof (tokens[4]);
  end_assets = atoi (tokens[5]);
  spent      = atof (tokens[6]);
  annuity    = atof (tokens[7]);
  socsec     = atof (tokens[8]);
  
  months = end_mo - beg_mo + 1;

  // Calculate inflation: Normalize by (12./months) in case not computing for an entire year
  // For now this is just a diagnostic (i.e. not used in other calculations)
  if (havecpi) {
    inflation = ((end_cpi - beg_cpi) /  beg_cpi) * (12./months) * 100.;
    printf ("Annualized inflation for year %s month %d through %d was %5.2lf%%\n",
	    year, beg_mo, end_mo, inflation);
  } else {
    printf ("Skipping inflation calc since one or both of beg_cpi/end_cpi were not specified\n");
  }

  // Calculate change of assets (% normalized to 1 year)
  // NOTE: change automatically INCLUDES income from outside sources (SS, interest-only, CSU med)
  // This is because those income source go directly into checking.
  change = (double) (end_assets - beg_assets);
  percent = 100. * change / (double) beg_assets * (12./months);
  if (months == 12)
    printf ("Assets changed from $%dK to $%dK which is a yearly change of %4.1lf%%\n",
	    beg_assets, end_assets, percent);
  else
    printf ("Assets changed from $%dK to $%dK which is an ESTIMATED yearly change of %5.2lf%%\n",
	    beg_assets, end_assets, percent);

  // spent money = pretax - pretax*taxrate. Solve for pretax
  pretax = spent / (1. - taxrate);

  // Adjust change for amount spent BEFORE taxes, and scale for partial year if necessary
  change = (change + pretax) * (12./months);
  percent = (100. * change) / beg_assets;
  if (months == 12) {
    printf ("Including spending of $%6.3lfK with assumed %d%% total income tax,\n"
	    "  asset yearly change is %4.1lf%%\n", pretax, (int) (taxrate*100), percent);
  } else {
    printf ("Spent $%4.1lfK over %d months, having added %d%% for assumed income tax\n",
	    pretax, months, (int) (taxrate*100));
    printf ("Including ESTIMATED YEARLY spending of $%6.3lfK with assumed 18%% total income tax,\n"
	    "  asset yearly change is %4.1lf%%\n", pretax*(12./months), percent);
  }

  // Calculate ramp to 0 based on current dollars (assumes investments keep up with inflation)
  // (12 - end_mo) accounts for possible partial-year at start of ramp
  // ramp_mo accounts for number of months into final year of calculation (can also be partial)
  // ((ramp_year - 1) - iyear)*12 is th number of full-year months in the interval
  iyear         = atoi (gettoken (year));
  nmonths       = (12 - end_mo) + ramp_mo + ((ramp_year - 1) - iyear)*12;
  permo_init    = (double) end_assets / nmonths + annuity       + socsec;
  annuity_final = inflation_effects (annuity, future_inflation, nmonths);
  permo_final   = (double) end_assets / nmonths + annuity_final + socsec;
  peryear_init  = 12.*permo_init;
  peryear_final = 12.*permo_final;

  printf ("\nThe following calcs assume year=%d numbers\n", iyear);
  printf ("Inflation effects are applied to fixed annuity\n");
  printf ("It is ASSUMED that soc sec income and investments will match inflation\n");
  printf ("  Thus ALL predicted numbers (including fixed annuity) are in %d dollars\n", iyear);
  printf ("Predicted draw numbers INCLUDE fixed annuity=$%-5.3lfK/mo and soc sec=$%-5.3lfK/mo\n",
	  annuity, socsec);
  printf ("Fixed annuity=$%-.3lfK year=%d mo=%d becomes $%-.3lfK year=%d mo=%d given inflation=%-.1lf%%\n",
	  annuity, iyear, end_mo, annuity_final, ramp_year, ramp_mo, future_inflation);
  printf ("Assets=$%dK plus annuities plus soc sec year=%d mo=%d ramp to $0 year=%d mo=%d given:\n",
	  end_assets, iyear, end_mo, ramp_year, ramp_mo);
  printf ("  Initial draw=$%-.1lfK/yr or $%-.2lfK/mo\n", peryear_init, permo_init);
  printf ("  Final draw=  $%-.1lfK/yr or $%-.2lfK/mo\n", peryear_final, permo_final);
  printf ("Initial draw of only INVESTED assets=$%-.1lfK/yr or $%-.2lfK/mo\n",
	  peryear_init - annuity*12. - socsec*12., permo_init - annuity - socsec);
  return 0;
}

int contains_question (const char *line)
{
  int i;
  
  for (i = 0; line[i] != '?' && i < LINESIZE; ++i);
  if (line[i] == '?')
    return 1;
  else
    return 0;
}

// create null-terminated token from input string (pointer into line)
char *gettoken (const char *str)
{
  int nc;
  char *token = 0;

  for (nc = 0; isdigit (str[nc]); ++nc);
  if (nc > 0) {
    token = (char *) malloc (nc+1);
    strncpy (token, str, nc);
    token[nc] = '\0';
  }
  return token;
}

int get9tokens (char *str, char **tokens)
{
  const char *delim = " \t";
  strtok (str, delim);
  if (!(tokens[0] = strtok(NULL, delim))) return -1;
  if (!(tokens[1] = strtok(NULL, delim))) return -1;
  if (!(tokens[2] = strtok(NULL, delim))) return -1;
  if (!(tokens[3] = strtok(NULL, delim))) return -1;
  if (!(tokens[4] = strtok(NULL, delim))) return -1;
  if (!(tokens[5] = strtok(NULL, delim))) return -1;
  if (!(tokens[6] = strtok(NULL, delim))) return -1;
  if (!(tokens[7] = strtok(NULL, delim))) return -1;
  if (!(tokens[8] = strtok(NULL, delim))) return -1;
  return 0;
}

// inflation_effects. Equation comes from exact formulation of "rule of 72" (see Wikipedia)
// Translated to yearly inflation effects the equation is:
// months/12 = log (initial/final) / log (1+interest) OR
// final = initial / exp (log (1 + interest)*months/12)
double inflation_effects (double initial_value, double future_inflation, int nmonths)
{
  return initial_value / exp (log (1. + 0.01*future_inflation)*nmonths/12.);
}
