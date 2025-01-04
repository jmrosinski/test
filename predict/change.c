#include <stdio.h>
#include <stdlib.h>  // atoi
#include <string.h>  // memset and string functions
#include <math.h>

// Compute change in assets from one point to another in the file yearly_data
// Include estimate of change in value for annuities that have begun but whose
// source doesn't provide current value info to the customer, e.g. TIAA

#define LINESIZE 128  // max size of input line

int get9tokens (char *, char **);            // parse all tokens from line

int main ()
{
  char line1[LINESIZE];          // beg input line
  char line2[LINESIZE];          // end input line
  char beg_yr[5], end_yr[5];     // beg, end years in character form
  int begyr, endyr;              // beg, end years
  FILE *fp;                      // file pointer
  float total_inflation;         // calculated inflation rate (fraction)
  float annualized_inflation;    // calculated inflation rate (fraction)
  int months;                    // number of months for calculation

  // Next 9 are parsed from input file
  int begmo, endmo;              // month of year to start and end calculations
  float begcpi, endcpi;          // CPI for beginning and ending times
  float begassets, endassets;    // assets (thousands) at start of begmo
  float change;                  // change in asset value
  float percent;
  char *tokens1[9], *tokens2[9]; // useful data from line

  // Read through entries, skipping those that don't start with a digit
  printf ("Enter start year\n");
  scanf ("%s", beg_yr);
  printf ("Enter end year\n");
  scanf ("%s", end_yr);

  begyr = atoi (beg_yr);
  endyr = atoi (end_yr);
  if (begyr < 2020) {
    printf ("beg year must be >= 2020\n");
    return 1;
  }
  if (endyr <= begyr) {
    printf ("end year must be later than beg year\n");
    return 1;
  }

  // Open required input file
  if ((fp = fopen ("./yearly_data", "r")) == NULL) {
    printf ("cannot open input file yearly_data\n");
    return 1;
  }

  // Read through input file until desired start and end lines are read
  do {
    memset (line1, 0, LINESIZE);
    if (fgets (line1, LINESIZE, fp) == NULL) {
      printf ("fgets reached EOF before finding begyr=%s\n", beg_yr);
      return 1;
    }
  } while (strncmp (line1, beg_yr, 4) != 0);
  do {
    memset (line2, 0, LINESIZE);
    if (fgets (line2, LINESIZE, fp) == NULL) {
      printf ("fgets reached EOF before finding end_yr=%s\n", end_yr);
      return 1;
    }
  } while (strncmp (line2, end_yr, 4) != 0);
  
  if (get9tokens (line1, tokens1) < 0 || get9tokens (line2, tokens2) < 0) {
    printf ("get9tokens failed\n");
    return 1;
  }

  begmo     = atoi (tokens1[3]);
  begcpi    = atof (tokens1[4]);
  begassets = atof (tokens1[5]);

  endmo     = atoi (tokens2[3]);
  endcpi    = atof (tokens2[4]);
  endassets = atof (tokens2[5]);

  months = 12*(endyr-begyr) + (endmo-begmo);

  total_inflation      = (endcpi - begcpi) /  begcpi;
  annualized_inflation = total_inflation * (12./months);
  printf ("Annualized inflation for year %d month %d through year %d month %d was %5.2f%%\n",
	  begyr, begmo, endyr, endmo, 100.*annualized_inflation);
  printf ("Total inflation for year %d month %d through year %d month %d was %5.2f%%\n",
	  begyr, begmo, endyr, endmo, 100.*total_inflation);

  // Adjust ending assets for started annuity values (e.g. TIAA) for which current value can no 
  // longer be tracked by user. Estimate value decrease due to inflation, and to life expectancy
  // decrease with time.
  // Calculations are only good to the nearest  year due to life expectancy tables.
  
#define NUM_ANNUITIES 1
#define MAXLIFE 101
  // const values below are specific to the individual
  const int birth_yr                           = 1958;
  const int annuity_startyr[NUM_ANNUITIES]     = {2023};
  const float start_cpi[NUM_ANNUITIES]         = {307.789};  // Sep. 2023 CPI value
  const float assets_annuitized[NUM_ANNUITIES] = {306.};     // K dollars
  float life_expect[MAXLIFE];
  int idx;
  float val;
  FILE *fp2;

  // File life_expectancy taken from: https://www.ssa.gov/oact/STATS/table4c6.html
  // Note: Male values were taken, but female values are also in the file
  if ( ! (fp2 = fopen ("./life_expectancy", "r"))) {
    printf ("Cannot open file life_expectancy for reading. Quitting\n");
    exit(1);
  }
  for (int i = 0; i < MAXLIFE; ++i)
    life_expect[i] = 1.e36;
  
  while (fscanf (fp2, "%d %f", &idx, &val) != EOF) {
    if (idx > 0 && idx < MAXLIFE) {
      life_expect[idx] = val;
    } else {
      printf ("idx=%d is invalild. Quitting\n", idx);
      exit(1);
    }
  }
  for (int n = 0; n < NUM_ANNUITIES; ++n) {
    if (begyr < annuity_startyr[n] && endyr >= annuity_startyr[n]) {
      float inflation                 = (endcpi - start_cpi[n]) / start_cpi[n];
      int annuity_startage            = annuity_startyr[n] - birth_yr;
      int endage                      = endyr - birth_yr;
      float life_expect_decrease_frac = (life_expect[annuity_startage] - life_expect[endage]) /
	                                life_expect[annuity_startage];
      float annuity_value             = assets_annuitized[n] / (1. + inflation);
      annuity_value                  /= 1. + life_expect_decrease_frac;
                                        
      endassets += annuity_value;
      printf ("Annuity %d init value=%4.1fK estimated to drop to %4.1fK over %d years %d to %d\n",
	      n, assets_annuitized[n], annuity_value, endyr-annuity_startyr[n],
	      annuity_startyr[n], endyr);
      printf ("...due to inflation =%4.1f%% and life expectancy decrease fraction=%4.1f%%\n",
	      100.*inflation, 100.*life_expect_decrease_frac);
    }
  }
  // Calculate change of assets (% normalized to 1 year)
  change  = endassets - begassets;
  percent = 100.*(12./months)*change / begassets;
  printf ("Unadjusted assets changed from $%4.1fK to $%4.1fK which is an annualized change of %+4.1f%%\n",
	  begassets, endassets, percent);
  
  change = endassets - begassets*(1. + total_inflation);
  percent = 100.*change / (float) endassets;
  printf ("Adjusted for inflation, in constant dollars, assets over the %d months changed by %+4.1f%%\n",
	  months, percent);
  printf ("This translates to an annualized value of %+4.1f%%\n", percent*(12./months));
  return 0;
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
