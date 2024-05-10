#include <stdio.h>
#include <stdlib.h>  // malloc, free
#include <ctype.h>   // atoi
#include <string.h>  // memset and string functions
#include <math.h>

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
  int begmo, endmo;              // month of year to start calculations
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

  // Adjust for the fact that $306K TIAA assets disappeared in Sept. 2023 due to annuitization
  // Also, decrease the 306K by adjusting it for inflation effects Sep 2023 to endyr
  if (begyr < 2023 && endyr >= 2023) {
    const int assets_annuitized_2023 = 306;     // K$ annuitized sept 2023
    const float sepcpi_2023          = 307.789; // cpi value at time annuitization
    float inflation                  = (endcpi - sepcpi_2023) / sepcpi_2023;
    float decrease                   = assets_annuitized_2023 / (1. + inflation);
    endassets += decrease;
    printf ("2023 annuity value=%dK assumed to have dropped to %4.1fK due to inflation=%4.1f%% Sep "
            "2023 through end date\n",
	    assets_annuitized_2023, decrease, 100.*inflation);
  }
  
  // Calculate change of assets (% normalized to 1 year)
  change  = endassets - begassets;
  percent = 100.*(12./months)*change / begassets;
  printf ("Assets changed from $%4.1fK to $%4.1fK which is an annualized change of %4.1f%%\n",
	  begassets, endassets, percent);
  
  change = endassets - begassets*(1. + total_inflation);
  percent = 100.*change / (float) endassets;
  printf ("Adjusted for inflation, in %d dollars, assets over the %d months changed by %4.1f%%\n",
	  endyr, months, percent);
  printf ("This translates to an annualized value of %4.1f%%\n", percent*(12./months));
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
