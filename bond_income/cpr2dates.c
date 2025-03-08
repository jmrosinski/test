#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#define LINESIZE 128  // max size of input line
#define MAXNAME 24    // max size of fund name

int get_fund_name (char *, char *);

  int main ()
{
  char line[2][LINESIZE];   // input line
  char file[2][64];         // 2 input files to compare
  FILE *fp[2];              // file pointers
  char fund_name[2][MAXNAME+1];
  double value[2];          // current fund value
  double mult[2];           // mult factor accounts for multiple accounts holding this security
  double monthly[2];        // monthly income
  double quarterly[2];      // quarterly income
  double yearly[2];         // yearly income
  double diff;              // fund difference in value between files
  double change;            // "diff" represented as percentage

  // Parse input lines assuming entries are: fund_name value mult monthly quarterly
  // If monthly > 0 use that, else quarterly
  for (int n = 0; n < 2; ++n) {
    // Open input files
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

  while (fgets (line[0], LINESIZE, fp[0]) != NULL) {
    if (get_fund_name (line[0], fund_name[0]) < 0) {
      printf ("Failed to find fund_name[0], quitting\n");
      return -1;
    }
    if (fgets (line[1], LINESIZE, fp[1]) == NULL) {
      printf ("Failed to get fund_name[1] matching fund_name[0]=%s\n. Quitting\n", fund_name[0]);
      return -1;
    }
    if (get_fund_name (line[1], fund_name[1]) < 0) {
      printf ("Failed to find fund_name[1], quitting\n");
      return -1;
    }
    if (strcmp (fund_name[0], fund_name[1]) != 0) {
      printf ("fundnames %s and %s don't match, quitting\n", fund_name[0], fund_name[1]);
      return -1;
    }

    for (int n = 0; n < 2; ++n) {
      if (sscanf (line[n], "%*s %lf %lf %lf %lf %lf",
		  &value[n], &mult[n], &monthly[n], &quarterly[n], &yearly[n]) != 5) {
	printf ("n=%d bad input line=%s\n", n, line[n]);
	return 1;
      }
      value[n]     *= mult[n];
      monthly[n]   *= mult[n];
      quarterly[n] *= mult[n];
      yearly[n]    *= mult[n];
    }

    // Currently no comparison stats computed for income
    if (mult[0] == mult[1]) {
      diff = value[1] - value[0];
      change = 100.* (diff / value[0]);
    } else {
      double scale = mult[0] / mult[1];
      printf ("NOTE: mult for %s changed from %.1lf to %.1lf. Scaling value2 by %.3lf\n",
	      fund_name[0], mult[0], mult[1], scale);
      diff = value[1]*scale - value[0];
      change = 100.* (diff / value[0]);
    }
    printf ("Value of %s changed (after applying mult=%.1lf)by %+.1lf dollars which is %+.1lf%%\n",
	    fund_name[0], mult[0], diff, change);
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
