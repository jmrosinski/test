#include <stdio.h>
#include <stdlib.h>  // malloc, free
#include <ctype.h>   // atoi
#include <string.h>  // memset and string functions

// To run: e.g. ./spent < expenses.2021.txt
// Parse stdin, looking for lines containing integer (dollars) starting in column 1,
// followed optionally by category name. Other input lines are skipped.
// Output: Last line is total expenditures. Per-category is also output
// NOTE: "total" should match "total_percat" exactly of there's a bug

#define SIZE 128  // max size of input line

static const char *unknown = "unknown";  // initial category is lines w/o category specified

struct category {
  char *name;     // category
  int nument;     // number of entries
  int total;      // sum of values
} *Cat;

int numcat = 0;  // number of categories found
int total = 0;   // grand total
int motot = -1;  // monthly total
char monam[10];  // name of current month
char *tstmo;

void initCat (void);
int getcatidx (const char *);               // find category index if known
void addcat (const int, const int);         // increment total for category
void newcat (const int, const char *);      // create new category and increment total
void categorize (const int, const char *);  // find category for textual entry
void print_results (void);
char *gettoken (const char *);              // create null-terminated token for textual entry
char *ismonth (char *);                     // input string is a month name

int main ()
{
  char line[SIZE];   // input line
  int dollars;       // cost (must start in column 1)

  // Create array of categories, initiallly size 1 for "unknown"
  initCat ();

  // Read through entries, skipping those that don't start with a digit
  memset (line, 0, sizeof line);
  while (fgets (line, SIZE, stdin) != NULL) {
    if ((tstmo = ismonth (&line[0]))) {
      if (motot > -1)
	printf ("Total for month=%9s= %5d\n", monam, motot);
      motot = 0;
      strcpy (monam, tstmo);
    } else if (isdigit (line[0])) {       // throw away lines which don't start with a digit
      dollars = atoi (&line[0]);
      total += dollars;
      motot += dollars;
      categorize (dollars, line);
    }
    memset (line, 0, sizeof line);
  }
  printf ("Total for month=%9s= %5d\n", monam, motot);
  print_results ();
  return 0;
}

void initCat ()
{
  Cat = malloc (1*sizeof (struct category));
  Cat[0].name = malloc (strlen (unknown)+1);
  Cat[0].nument = 0;
  strcpy (Cat[0].name, unknown);
  numcat = 1;
}

// find index matching token (from line). If not found return -1
int getcatidx (const char *token)
{
  for (int n = 0; n < numcat; ++n) {
    if (strcmp (Cat[n].name, token) == 0) {
      return n;
    }
  }
  return -1;
}

// sum dollars into category
void addcat (const int dollars, const int idx)
{
  ++Cat[idx].nument;
  Cat[idx].total += dollars;
}

// create new category and initialize nument, dollars
void newcat (const int dollars, const char *name)
{
  int nc;
  struct category *tmp;

  tmp = (struct category *) realloc (Cat, (numcat+1)*sizeof (struct category));
  Cat = tmp;
  Cat[numcat].name = malloc (strlen (name) + 1);
  strcpy (Cat[numcat].name, name);
  Cat[numcat].nument = 1;
  Cat[numcat].total = dollars;
  ++numcat;
}

// create null-terminated token from input string (pointer into line)
char *gettoken (const char *str)
{
  int nc;
  char *token = 0;

  for (nc = 0; isalnum (str[nc]) || str[nc] == '_'; ++nc);
  if (nc > 0) {
    token = (char *) malloc (nc+1);
    strncpy (token, str, nc);
    token[nc] = '\0';
  }
  return token;
}

// parse input line (1st entry dollars was already parsed by caller)
void categorize (const int dollars, const char *line)
{
  int i;
  int idx;
  char *cat;

  if (! isdigit (line[0]))  // consider only lines starting with a digit (dollars)
    return;
  
  for (i = 0; isdigit (line[i]); ++i);  // get past amount
  while (isspace (line[i]) && line[i] != '\n')             // get to 1st char of category
    ++i;
  if (line[i] == '\n') {           // no category specified so put in unknown
    addcat (dollars, 0);           // unknown is index 0
#ifdef DEBUG
    printf ("added %d dollars to cat %s cat total=%d\n", dollars, Cat[0].name, Cat[0].total);
#endif
  } else {
    cat = gettoken (&line[i]);     // retrieve the category name
    idx = getcatidx (cat);         // find existing category idx, or -1 if not found
    if (idx > -1) {                // add to existing category
      addcat (dollars, idx);
#ifdef DEBUG
      printf ("added %d dollars to cat %s cat total=%d\n", dollars, cat, Cat[idx].total);
#endif
    } else {                       // create new category and set to dollars
      newcat (dollars, cat);
#ifdef DEBUG
      printf ("set %d dollars to new cat %s\n", dollars, cat);
#endif
    }
    free (cat);
  }
}
 
void print_results ()
{
  int totent = 0;
  int total_percat = 0;
  for (int n = 0; n < numcat; ++n) {
    printf ("nument=%3d dollars=%6d cat=%s\n", Cat[n].nument, Cat[n].total, Cat[n].name);
    total_percat += Cat[n].total;
    totent += Cat[n].nument;
  }
  total        += totent/2;  // adjust for lopping off fraction of each entry
  total_percat += totent/2;  // adjust for lopping off fraction of each entry
  printf ("total=%d total_percat=%d\n", total, total_percat);
}

#define STRMATCH(X,Y) (strcmp((X),(Y)) == 0)
char *ismonth (char *str)
{
  if (isalpha (str[0])) {
    char *monam = strtok (str, "\n");
    if (STRMATCH (monam, "January") || STRMATCH (monam, "February") || STRMATCH (monam, "March") ||
	STRMATCH (monam, "April")   || STRMATCH (monam, "May")      || STRMATCH (monam, "June") ||
	STRMATCH (monam, "July")    || STRMATCH (monam, "August")   || STRMATCH (monam, "September") ||
	STRMATCH (monam, "October") || STRMATCH (monam, "November") || STRMATCH (monam, "December")) {
      return monam;
    }
  }
  return NULL;
}
