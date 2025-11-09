#include <stdio.h>
#include <stdlib.h>  // atoi, atof
#include <string.h>  // memset and string functions
#include <math.h>

// Estimate change in invested assets from one point to another in the file yearly_data
// Include estimate of change in value for annuities that have begun but whose
// source doesn't provide current value info to the customer, e.g. TIAA

#define LINESIZE 128  // max size of input line

int get9tokens (char *, char **);            // parse all tokens from line

int main ()
{
  const double tot_taxrate = 0.18; // assumed aggregate total tax rate (fraction)
  const double top_taxrate = 0.28; // roth_conv * top_taxrate gets added back to invest income

  char line[LINESIZE];             // input line
  char yr[5];                      // year of calculation (char)
  int year;                        // year of calculation (int)
  FILE *fp;                        // file pointer
  double total_inflation;          // calculated inflation rate (fraction)
  double annualized_inflation;     // calculated inflation rate (fraction)
  int nmo;                         // intermediate number of months
  int months;                      // number of months for calculation
  double rothconv;                 // total Roth conversions from 1st thru 2nd entries
  double taxed_rothconv;           // rothconv * top_taxrate
  double spent;                    // total amt spent between 1st and 2nd entries
  double spent_inctax;             // "spent" modified to include income taxed amount
  double socsec;                   // social security earned between 1st and 2nd entries
  double annuity;                  // annuity (monthly)
  double annuity_tot;              // annuity accumulated over months
  double invest_growth;            // change modified by socsc, annunity, expenses, Roth

  // These are parsed from input file
  int begmo, endmo;                // begin and end month of year
  double begcpi, endcpi;           // CPI for beginning and ending times
  double begassets, endassets;     // assets (thousands) at end of 1st and 2nd entries
  double change;                   // change in asset value
  char *tokens[9];                 // useful data from line

  // Read through entries, skipping those that don't start with a digit
  printf ("Enter year of calculation\n");
  scanf ("%s", yr);

  year = atoi (yr);
  if (year < 2020) {
    printf ("beg year must be >= 2020\n");
    return 1;
  }

  printf ("Enter total Roth conversion (thousands of $) for year=%d\n", year);
  scanf ("%lf", &rothconv);

  // Open required input file
  if ((fp = fopen ("./yearly_data", "r")) == NULL) {
    printf ("cannot open input file yearly_data\n");
    return 1;
  }

  // Read through input file1 until desired start line is found
  do {
    memset (line, 0, LINESIZE);
    if (fgets (line, LINESIZE, fp) == NULL) {
      printf ("fgets reached EOF before finding year=%s\n", yr);
      return 1;
    }
  } while (strncmp (line, yr, 4) != 0);

  if (get9tokens (line, tokens) < 0) {
    printf ("get9tokens1 failed\n");
    return 1;
  }
  // tokens[0] will be the first token AFTER the year, because (I think) the first delimiter
  // (space or tab) occurs AFTER the year
  begmo     = atoi (tokens[0]);
  begcpi    = atof (tokens[1]);
  begassets = atof (tokens[2]);
  endmo     = atoi (tokens[3]);
  endcpi    = atof (tokens[4]);
  endassets = atof (tokens[5]);
  spent     = atof (tokens[6]);
  annuity   = atof (tokens[7]);         // ASSUME annuity is constant across years
  months    = endmo - begmo + 1;        // total number of months
  socsec    = months*atof (tokens[8]);  // total social security income

  total_inflation      = (endcpi - begcpi) /  begcpi;
  annualized_inflation = total_inflation * (12./months);
  printf ("Annualized inflation for year %d month %d through month %d was %5.2f%%\n",
	  year, begmo, endmo, 100.*annualized_inflation);
  printf ("Total inflation for year %d month %d through month %d was %5.2f%%\n",
	  year, begmo, endmo, 100.*total_inflation);

  annuity_tot    = annuity*months;
  spent_inctax   = spent*(1. + tot_taxrate);
  taxed_rothconv = rothconv*top_taxrate;
  change         = endassets - begassets;
  invest_growth  = change - annuity_tot - socsec + spent_inctax + taxed_rothconv;
  printf ("Investment change year=%d mo=%d through mo=%d is %+.1lfK\n",
	  year, begmo, endmo, change);
  printf ("To estimate investment growth, subtractions from change are annuity=%.1lfK, "
	  "social security=%.1lfK\n", annuity_tot, socsec);
  printf ("  Additions are spent_inctax=%.1lfK, taxed_rothconv=%.1lfK\n",
	  spent_inctax, taxed_rothconv);
  printf ("  Total change of %+.1lfK represents an annualized value of %+.1lf%%\n", invest_growth,
	  (100.*invest_growth)/begassets);
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
