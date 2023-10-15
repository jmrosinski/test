#include <stdio.h>
#include <stdlib.h>  // malloc, free
#include <ctype.h>   // atoi
#include <string.h>  // memset and string functions

// To run: e.g. ./predict
// User input asked: year to examine, estimated death year, month
// Data file read is "yearly_data"
// Output: annualized inflation
//         % change in assets (accounting for $ spent that year)
//         $ can take out to ramp to 0 at death date
// NOTE: program gives up if any entries in file are "?" or year not found

#define LINESIZE 128  // max size of input line

char *gettoken (const char *);          // create null-terminated token for textual entry
int get8tokens (char *, char **);       // parse all tokens from line
int contains_question (const char *);   // see if input contains "?"

int main ()
{
  char line[LINESIZE];   // input line
  char year[5];
  int iyear;             // integer version of year
  int ret;
  FILE *fp;
  float inflation;       // calculated inflation rate (%)
  int months;            // number of months for calculation (normally 12)
  int havecpi;           // flag indicating both start_cpi and end_cpi were specified

  // Next 7 are parsed from input line
  int start_mo;      // month of year to start calculations
  float start_cpi=0; // CPI for PREIOUS month to start_mo
  int start_assets;  // assets (thousands) at start of start_mo
  int end_mo;        // month of year to end calculations
  float end_cpi=0;   // CPI for end_mo
  int end_assets;    // assets (thousands) at end of end_mo
  float spent;       // thousands spent from start_mo through end_mo
  float annuity;     // monthly annuity income (started UCAR 9/2023)

  int ramp_year;     // year to ramp to 0
  int ramp_mo;       // month to ramp to 0
  float change;      // change in asset value
  int nmonths;       // number of months to ramp to zero
  float percent;
  float permo;       // amount can withdraw per month
  float peryear;     // amount can withdraw per year
  char *tokens[8];   // useful data from line

  // Read through entries, skipping those that don't start with a digit
  printf ("Enter input year\n");
  scanf ("%4s", year);
  printf ("Enter end year and month for prediction ramp\n");
  scanf ("%d %d", &ramp_year, &ramp_mo);

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

  if (get8tokens (line, tokens) < 0) {
    printf ("get8tokens failed\n");
    return 1;
  }

  havecpi = 1;
  start_mo = atoi (tokens[0]);
  if (*tokens[1] == '?')
    havecpi = 0;
  else
    start_cpi  = atof (tokens[1]);

  start_assets = atoi (tokens[2]);
  end_mo       = atoi (tokens[3]);
  if (*tokens[4] == '?')
    havecpi = 0;
  else
    end_cpi  = atof (tokens[4]);
  end_assets = atoi (tokens[5]);
  spent      = atof (tokens[6]);
  annuity    = atof (tokens[7]);
  
  months = end_mo - start_mo + 1;

  // Calculate inflation: Normalize by (12./months) in case not computing for an entire year
  // For now this is just a diagnostic (i.e. not used in other calculations)
  if (havecpi) {
    inflation = ((end_cpi - start_cpi) /  start_cpi) * (12./months) * 100.;
    printf ("Annualized inflation for year %s month %d through %d was %5.2f%%\n",
	    year, start_mo, end_mo, inflation);
  } else {
    printf ("Skipping inflation calc since one or both of start_cpi/end_cpi were not specified\n");
  }

  // Calculate change of assets (% normalized to 1 year)
  // NOTE: change automatically INCLUDES income from outside sources (SS, interest-only, CSU med)
  // This is because those income source go directly into checking.
  change = (float) (end_assets - start_assets);
  percent = 100. * change / (float) start_assets * (12./months);
  if (months == 12)
    printf ("Assets changed from $%dK to $%dK which is a yearly change of %4.1f%%\n",
	    start_assets, end_assets, percent);
  else
    printf ("Assets changed from $%dK to $%dK which is an ESTIMATED yearly change of %4.1f%%\n",
	    start_assets, end_assets, percent);

  // Estimate before-taxes money spent as spent*1.25
  spent *= 1.25;

  // Adjust change for amount spent BEFORE taxes, and scale for partial year if necessary
  change = (change + spent) * (12./months);
  percent = (100. * change) / start_assets;
  if (months == 12) {
    printf ("Including spending of $%6.3fK with assumed 25%% total income tax,\n"
	    "  asset yearly change is %4.1f%%\n", spent, percent);
  } else {
    printf ("Spent $%4.1fK over %d months, having added 25%% for assumed income tax\n",
	    spent, months);
    printf ("Including ESTIMATED YEARLY spending of $%6.3fK with assumed 25%% total income tax,\n"
	    "  asset yearly change is %4.1f%%\n", spent*(12./months), percent);
  }

  // Calculate ramp to 0 WITHOUT taking inflation into account
  // (12 - end_mo) accounts for possible partial-year at start of ramp
  // ramp_mo accounts for number of months into final year of calculation (can also be partial)
  // ((ramp_year - 1) - iyear)*12 is th number of full-year months in the interval
  iyear = atoi (gettoken (year));
  nmonths = (12 - end_mo) + ramp_mo + ((ramp_year - 1) - iyear)*12;
  permo = (float) end_assets / nmonths + annuity;
  peryear = 12.*permo;
  printf ("Assets=%d at end of month=%2d of year=%d will ramp to 0 at\n"
	  "                      month=%2d of year=%d by drawing "
	  "$%5.1fK per year or $%-5.2fK per mo\n",
	  end_assets, end_mo, iyear, ramp_mo, ramp_year, peryear, permo);
  printf ("This INCLUDES assumed lifetime annuity income of $%-5.3fK per mo\n", annuity);
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

int get8tokens (char *str, char **tokens)
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
  return 0;
}
