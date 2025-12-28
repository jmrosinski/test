#include <stdio.h>
#include <stdlib.h>  // atoi, atof
#include <string.h>  // memset and string functions
#include <math.h>

// Estimate investment growth for a given year (2020-present).
//   Required input file: "yearly_data" which includes year and various required inputs such as
//   month, assets, CPI-U, spending, and change in S&P500
// Assumptions:
//   spending comes first from fixed income assets (soc sec, annuity, dcp), then as needed from
//   investments.
// "diff" = "spent" - "fixed_income": fraction of "spent" which came from investments
//   If negative, some of fixed_income actually got added to investments.
// "delta_tax_april" = "taxpaidfor_prvyr[thisyear]" - "taxpaidfor_prvyr[thisyear+1]"
//   Changes invest_growth since taxes paid April current year actually should have been
//     paid from assets in the previous year.
// Invest_growth = net_worth_growth + diff + delta_tax_april.
//   diff positive means some of money spent came from investments, so adds back to Invest_growth
//   diff negative means some of fixed_income ended in investments, so subtracts from Invest_growth
//   Note delta_tax_april can be positive or negative

#define LINESIZE 128  // max size of input line pulled from file "yearly_data"
#define NUMYRS 7
const int idx_map[NUMYRS] = {2020,2021,2022,2023,2024,2025,2026}; // supported tax years

int main ()
{
  int get10tokens (char *, char **); // parse all tokens from line
  int get_yridx (int);               // retrieve array index corresponding to year

  const char *infile = "yearly_data";
  // Retired June, 2020. Annuity interest-only started July, 2020. "annuity" monthly 
  //   income therefore averaged over 12 months. Varied somewhat month to month
  // Started social security June, 2021. 1186. = 2033.*(7/12)
  // Turned 65 May, 2023. Began medicare, premiums taken from social security.
  //   Subsequent years ss income decreased due to IRMAA, which explains ss decrease in 2023
  //   IRMAA explains the decreases in 2025, 2026 as I became more aggressive in doing Roth
  //   conversions.
  const double unk = -99999.;  // unknown value
  // socsec, annuity, and dcp_refund are the MONTHLY amounts put into checking acct, in other words
  //   AFTER withholding or medicare. These numbers will get converted to yearly values
  double socsec[NUMYRS] =     {0.,   1186., 2152., 2127., 2240., 1938., 1848.};
  double annuity[NUMYRS] =    {659., 1080., 1338., 1557., 1912., 1925., 1920.};
  double dcp_refund[NUMYRS] = {84.,  84.,   84.,   259.,  259.,  259.,  259.};
  // tax_prvyr are the fed+state amount sent to gov't that apply to the PREVIOUS year
  double tax_prvyr[NUMYRS] = {unk,  8693., 5007., 3699., 5076., 11838., unk};
  int idx, idxp1;                  // year (and year+1) index into socsec, annuity, dcp_refund
  char line[LINESIZE];             // input line
  char yr[5];                      // year of calculation (char)
  int year;                        // year of calculation (int)
  FILE *fp;                        // file pointer
  double total_inflation;          // calculated inflation rate (fraction)
  int months;                      // number of months for calculation (likely < 12 for current yr)
  double spent;                    // total amt spent during the year
  double change;                   // change in asset value
  double invest_growth;            // change modified by socsc, annunity, expenses, IRA conv
  double fixed_income;             // socsec + annuity + dcp_refund for the year in question
  double diff;                     // spent - fixed_income
  double delta_tax_april;          // difference between tax checks written year - year+1
  double delta_sp500;              // change in s&p500 for that year

  // These are parsed from input file
  int begmo, endmo;                // begin and end month of year
  double begcpi, endcpi;           // CPI for beginning and ending times
  double begassets, endassets;     // assets (thousands) at end of 1st and 2nd entries
  char *tokens[9];                 // data items from relevant line of file

  // Read through entries, skipping those that don't start with a digit
  printf ("Enter year of calculation\n");
  scanf ("%s", yr);

  year = atoi (yr);
  if ((idx = get_yridx (year)) < 0) {
    printf ("index for year=%d not found. Quitting\n", year);
    return -1;
  }

  // Open required input file
  if ((fp = fopen (infile, "r")) == NULL) {
    printf ("cannot open input file %s\n", infile);
    return 1;
  }

  // Read through input infile until desired start line is found
  do {
    memset (line, 0, LINESIZE);
    if (fgets (line, LINESIZE, fp) == NULL) {
      printf ("fgets reached EOF before finding year=%s\n", yr);
      return 1;
    }
  } while (strncmp (line, yr, 4) != 0);

  if (get10tokens (line, tokens) < 0) {
    printf ("get10tokens failed\n");
    return 1;
  }
  // tokens[0] will be the first token AFTER the year, because (I think) the first delimiter
  // (space or tab) occurs AFTER the year
  begmo       = atoi (tokens[0]);
  begcpi      = atof (tokens[1]);
  begassets   = atof (tokens[2]);
  endmo       = atoi (tokens[3]);
  endcpi      = atof (tokens[4]);
  endassets   = atof (tokens[5]);
  spent       = atof (tokens[6]);
  delta_sp500 = atof (tokens[9]);

  months      = endmo - begmo + 1;     // total number of months

  // For relevant items, convert array value from $ to thousands of $,
  // and convert from monthly to yearly (or "so far this year" if months < 12)
  socsec[idx]     *= months*.001;
  annuity[idx]    *= months*.001;
  dcp_refund[idx] *= months*.001;
  if ((idxp1 = get_yridx (year+1)) < 0) {
    delta_tax_april = .001*tax_prvyr[idx];;
    printf ("index for year+1=%d not found. Assuming 0 fed+state tax check written for year=%d\n",
	    year+1, year);
  } else if (tax_prvyr[idxp1] == unk) {
    delta_tax_april = .001*tax_prvyr[idx];
    printf ("fed+state tax check written for year+1=%d is unknown. Assuming 0\n", year+1);
  } else {
    delta_tax_april = .001*(tax_prvyr[idx] - tax_prvyr[idxp1]);
  }

  total_inflation = (endcpi - begcpi) /  begcpi;
  printf ("Total inflation for year %d month %d through month %d was %5.2lf%%\n",
	  year, begmo, endmo, 100.*total_inflation);
  if (months != 12) {
    double annualized_inflation = total_inflation * (12./months);
    printf ("Annualized inflation for year %d month %d through month %d was %5.2lf%%\n",
	    year, begmo, endmo, 100.*annualized_inflation);
  }

  change = endassets - begassets;    // total change in assets
  if (year == 2023) {
    printf ("In Sept. 2023 TIAA annuity was zeroed and changed to lifetime monthly stipend.\n"
	    "  Adding 306. to asset change (change) to account for this initial annuity value\n");
    change += 306.;
  }

  printf ("\nResults for calendar year %d month %d through month %d:\n", year, begmo, endmo);
  printf ("Raw investments change: $%+.1lfK\n", change);
  // annuity, social security and dcp_refund income get subtracted from asset change since 
  //   they are fixed income not investments
  fixed_income = annuity[idx] + socsec[idx] + dcp_refund[idx];
  printf ("fixed_income (annuity + socsec + dcp_refund): $%.3lfK\n", fixed_income);
  // if spent exceeds fixed_income, increase invest_growth by the difference since the excess of
  //   spent over fixed_income came from investments
  // if fixed_income exceeds spent, decrease invest_growth by the difference since the excess of
  //   fixed_income over spent got added to investments, which is not considered "growth"
  diff = spent - fixed_income;
  printf ("Diff (diff) between spent and fixed income: $%+.3lfK\n", diff);

  printf ("Diff (delta_tax_april) bet tax paid this year for prv year and tax paid next year for "
	  "this year: $%+.3lfK\n", delta_tax_april);
  invest_growth = change + diff + delta_tax_april;
  
  printf ("Estimated investment growth (change + diff + delta_tax_april): $%+.1lfK or %.1lf%%\n",
	  invest_growth, (100.*invest_growth)/begassets);
  if (months < 12)
    printf ("  Investment growth estimate as yearly change: $%+.3lf%%\n",
	    (12./months)*(100.*invest_growth)/begassets);
  printf ("Change in S&P500 over %d months: %+.2lf%%\n", months, delta_sp500);
  return 0;
}

int get10tokens (char *str, char **tokens)
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
  if (!(tokens[9] = strtok(NULL, delim))) return -1;
  return 0;
}

int get_yridx (int year)
{
  for (int i=0; i<NUMYRS; ++i) {
    if (idx_map[i] == year)
      return i;
  }
  return -1;  // not found
}
