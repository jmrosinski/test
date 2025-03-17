#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define LINESIZE 128  // max size of input line
#define MAXNAME 24    // max size of fund name

int get_fund_name (char *, char *);
double getyears (char *, char *);
int get_same_entry (char *, FILE *, char *);
int jrstrtoi (const char *, const int);
  
int main ()
{
  char line[2][LINESIZE];   // input line
  char file[2][64];         // 2 input files to compare
  FILE *fp[2];              // file pointers
  char fund_name[MAXNAME+1];
  double value[2];          // current fund value
  double basis[2];          // cost basis of the fund
  double delta_value;       // change in value between dates
  double delta_basis;       // change in bassis between dates
  double years;             // years between the 2 input files
  double change;            // "diff" represented as percentage
  double percent;           // % change

  // Open files and skip first 2 entries
  for (int n = 0; n < 2; ++n) {
    printf ("Enter file[%d]\n", n);
    scanf ("%s", file[n]);
    if ((fp[n] = fopen (file[n], "r")) == NULL) {
      printf ("cannot open input file[%d] %s\n", n, file[n]);
      return 1;
    }
    printf ("Successfully opened file[%d]=%s\n", n, file[n]);

    // Skip over exactly 2 input header lines from each file
    fgets (line[n], LINESIZE, fp[n]);
    fgets (line[n], LINESIZE, fp[n]);
  }

  // Determine time difference (days) between input files based on their name ending in yymmdd
  if ((years = getyears (file[0], file[1])) == 0.) {
    printf ("Could not determine delta-t between files %s and %s Quitting\n", file[0], file[1]);
    return 1;
  }
  printf ("Computing fund performance files=%s %s time period=%.3lf years\n",
	  file[0], file[1], years);

  // Parse input lines assuming entries are: fund_name value basis
  while (fgets (line[0], LINESIZE, fp[0]) != NULL) {
    if (get_fund_name (line[0], fund_name) < 0) {
      printf ("Failed to find fund_name in first file, quitting\n");
      return -1;
    }
    // Find matching line in second file
    if (get_same_entry (line[1], fp[1], fund_name) != 0) {
      printf ("Failed to find fund_name=%s in second file, skipping\n", fund_name);
      break;
    }

    for (int n = 0; n < 2; ++n) {
      if (sscanf (line[n], "%*s %lf %lf", &value[n], &basis[n]) != 2) {
	printf ("n=%d bad input line=%s, giving up\n", n, line[n]);
	return 1;
      }
    }
    delta_value = value[1] - value[0];
    delta_basis = basis[1] - basis[0];
    change = delta_value - delta_basis;
    percent = (change/value[0]) * (100./years);
    printf ("fund=%s current value=%.1lf change=%.1lf yearly_change=%.2lf%% basis change=%.1lf\n",
	    fund_name, value[1], change, percent, delta_basis);
  }
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

double getyears (char *file0, char *file1)
{
  int yr0, yr1;
  int mo0, mo1;
  int dy0, dy1;
  double years;  // return value
  const int daysmo[13] = {0,31,59,90,120,151,181,212,243,273,304,334,365};

  if (strncmp (file0, "securities.", 11) != 0 || strncmp (file1, "securities.", 11) != 0) {
    printf ("getyears: Both input file names must start with 'securities.'\n");
    return 0.;
  }
  // C functions atoi, strtol, etc. are too cumbersome. Use Fortran.
  // ASSUME Fortran name mangling is single-underscore. Not portable but who cares.
  yr0 = jrstrtoi (&file0[11], 2);
  mo0 = jrstrtoi (&file0[13], 2);
  dy0 = jrstrtoi (&file0[15], 2);

  yr1 = jrstrtoi (&file1[11], 2);
  mo1 = jrstrtoi (&file1[13], 2);
  dy1 = jrstrtoi (&file1[15], 2);

  // Not perfect but close enough
  years = (yr1 - yr0) + (daysmo[mo1-1] - daysmo[mo0-1])/365. + (dy1 - dy0)/365.;
  return years;
}

// Find spot in fp matching fund_name. Store the input line in "line"
int get_same_entry (char *line, FILE *fp, char *fund_name)
{
  int nchars = strlen (fund_name);

  rewind (fp);
  while (fgets (line, LINESIZE, fp) != NULL) {
    if (strncmp (fund_name, line, nchars) == 0 && isspace (line[nchars])) {
      return 0; // Success. line contains matching comparison to fund_name
    }
  }
  return -1; // Failed to find matching entry
}

// Pull nc characters from a string and use atoi() to convert to int
int jrstrtoi (const char *instr, const int nc)
{
  char outstr[nc+1];
  for (int i=0; i<nc; ++i)
    outstr[i] = instr[i];
  outstr[nc] = '\0';
  return atoi (outstr);
}
