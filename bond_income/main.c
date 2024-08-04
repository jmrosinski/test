#include <stdio.h>
#include <ctype.h>

#define LINESIZE 128  // max size of input line
#define MAXNAME 24    // max size of fund name

int get_fund_name (char *, char *);

  int main ()
{
  char line[LINESIZE]; // input line
  char *infile = "bond_funds";
  FILE *fp;
  char fund_name[MAXNAME+1];
  double value;       // current fund value
  double monthly;     // monthly income
  double quarterly;   // quarterly income
  double yearly;      // yearly income
  double income;      // yearly income (computed)
  double rate;        // yearly payout rate
  double v_tot = 0.;  // total invested in bond funds
  double i_tot = 0.;  // total income from bond funds
  int nument = 0;     // number of entries found

  // Open required input file
  if ((fp = fopen (infile, "r")) == NULL) {
    printf ("cannot open input file %s\n", infile);
    return 1;
  }
  printf ("Successfully opened infile=%s\n", infile);

  // Skip over exactly 2 input header lines
  fgets (line, LINESIZE, fp);
  fgets (line, LINESIZE, fp);

  // Parse input lines assuming entries are: fund_name value monthly quarterly
  // If monthly > 0 use that, else quarterly
  while (fgets (line, LINESIZE, fp) != NULL) {
    if (get_fund_name (line, fund_name) < 0)
      return -1;
    if (sscanf (line, "%*s %lf %lf %lf %lf", &value, &monthly, &quarterly, &yearly) != 4) {
      printf ("bad input line=%s\n", line);
      return 1;
    }
    ++nument;
    v_tot += value;
    
    if (monthly > 0.) {
      income = monthly*12.;
      i_tot += income;
      rate = 100.* (income / value);
      printf ("fund=%24s value=%.1lf yearly income=%.1lf rate=%.1lf%%\n",
	      fund_name, value, income, rate);
    } else if (quarterly > 0.) {
      income = quarterly*4.;
      i_tot += income;
      rate = 100.* (income / value);
      printf ("fund=%24s value=%.1lf yearly income=%.1lf rate=%.1lf%%\n",
	      fund_name, value, income, rate);
    } else if (yearly > 0.) {
      income = value;
      i_tot += income;
      rate = 100.* (income / value);
      printf ("fund=%24s value=%.1lf yearly income=%.1lf rate=%.1lf%%\n",
	      fund_name, value, income, rate);
    } else {
      printf ("fund=%s monthly quarterly or yearly income not found. Skipping \n", fund_name);
    }
  
  }
  printf ("%d entries were found value total=%.1lf income total=%.1lf\n", nument, v_tot, i_tot);
  
  return 0;
}

int get_fund_name (char *line, char *fund_name)
{
  int len;
  
  while (! isalpha ((int) *line))
    ++line;
  for (len = 0; isalnum (line[len]) || line[len] == '_'; ++len);
  if (len > MAXNAME) {
    printf ("length of 1st token in %s=%d exceeds %d. Aborting\n", line, len, MAXNAME);
    return -1;
  }
  for (int i = 0; i < len; ++i)
    fund_name[i] = *line++;
  fund_name[len] = '\0';
  return 0;
}
