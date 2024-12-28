#include <stdio.h>
#include <math.h>

#define NUMYRS 2
#define NUMBRACKETS 5
const int idx_map[NUMYRS] = {2023,2024}; // supported tax years
const double kicksinat[NUMYRS][NUMBRACKETS] = {{0.,11., 44.725,95.375, 182.1},
					       {0.,11.6,47.15, 100.525,191.95}};

int main()
{
  // All dollar-based floating point settings and inputs are in thousands
  const double std_deduction[NUMYRS] = {15.7,16.45};
  const double taxrate [NUMBRACKETS] = {10.,12.,22.,24.,32.}; // tax rate (%) for each bracket
  const double cgrate = 15.; // Cap gains rate ASSUMED to be this percent
  int idx;               // user input: index into arrays to match "year"
  int year;              // user input: year to estimate federal taxes
  int topbracketidx;     // index of top income bracket
  double income;         // user input: total income not including cap gains/qualified dividends
  double capgains;       // user input: capital gains net
  double odiv;           // user input: ordinary dividends
  double qdiv;           // user input: qualified dividends
  double capg_qdiv;      // capital gains + qualified dividends (assumed taxed at cgrate)
  double agi;            // adjusted gross income (income + ordinary dividends + cap gains)
  double taxable_income; // agi - std deduction
  double taxed_fromtable;      // taxable_income - capg_qdiv
  double tax;            // estimated federal tax
  double cgtax;          // tax on cap gains+qdiv
  // CO-specific items
  const double COtaxrate [NUMYRS] = {4.4, 4.25}; // CO tax rate (%) for each year
  const double ssreduce = 24.;   // Age 65 and above CO allows up to 24K removal of SS+annuity
  const double maxcharity = 0.5; // CO allows up to $500 chartiable deduction
  const double tabor[NUMYRS] = {0.8, 0.277}; // 0.277 is for AGI between $105K and $166K
  double usbondincome;           // CO doesn't tax US bond income
  double reductions;
  double COincome;
  double COtax;

  // function prototypes
  int get_yridx (int);                 // find index to match year
  int get_topbracketidx (int, double); // find index of top tax bracket

  printf ("Enter tax year for calculation:\n");
  scanf ("%d", &year);
  if ((idx = get_yridx (year)) < 0) {
    printf ("index for year=%d not found. Quitting\n", year);
    return -1;
  }
  printf ("Estimating federal tax for year=%d\n", year);
  printf ("NOTE: All requested input is expected in floating point thousands of dollars\n");
  
  printf ("Enter taxable income excluding capgains and dividends (maybe 2b+4b+5b+6b of 1040)\n");
  scanf ("%lf", &income);

  printf ("Enter ordinary dividends (maybe line 3b of 1040)\n");
  scanf ("%lf", &odiv);

  printf ("Enter qualified dividends (maybe line 3a of 1040)\n");
  scanf ("%lf", &qdiv);

  printf ("Enter net capital gains (maybe line 16 of schedule D)\n");
  scanf ("%lf", &capgains);

  capg_qdiv = capgains + qdiv;
  printf ("cap gains + qualified dividends=$%.3lfK ASSUMED to be taxed at %.3lf%%\n",
	  capg_qdiv, cgrate);
  
  agi = income + odiv + capgains;
  printf ("adjusted gross income=$%.3lfK\n", agi);
  
  taxable_income = agi - std_deduction[idx];
  printf ("taxable income before subtracting cg and qdiv=$%.3lfK\n", taxable_income);

  if ((topbracketidx = get_topbracketidx (idx, taxable_income)) < 0) {
    printf ("Cannot find top tax bracket index. Quitting\n");
    return -1;
  }
  printf ("Top tax bracket begins at $%.3lfK\n", kicksinat[idx][topbracketidx]);

  // Remove capg_qdiv from taxable income because rate on capg_qdiv is lower.
  // It will be added later.
  taxed_fromtable = taxable_income - capg_qdiv;
  if (taxed_fromtable > kicksinat[idx][NUMBRACKETS-1]) {
    printf ("Need to add a bracket: taxed_fromtable=$%.3lfK exceeds top limit of $%.3lfK Quitting\n",
	    taxed_fromtable, kicksinat[idx][NUMBRACKETS-1]);
    return -1;
  }

  // Loop over tax brackets to figure total tax
  tax = 0.;
  for (int i = 0; i < NUMBRACKETS-1 && taxed_fromtable >= kicksinat[idx][i]; ++i) {
    double limit = fmin (kicksinat[idx][i+1], taxed_fromtable);
    tax += (limit - kicksinat[idx][i]) * 0.01*taxrate[i];  // 0.01 converts percent to fraction
    if (taxed_fromtable < kicksinat[idx][i+1])
      break;
  }
  printf ("tax on pure income=$%.3lfK\n", tax);
  // Add tax on capital gains+qualified dividends
  cgtax = capg_qdiv*0.01*cgrate;
  printf ("tax on cap gains+qdiv=$%.3lfK\n", cgtax);
  tax += cgtax;
  printf ("federal tax=$%.3lfK\n", tax);

  // Begin CO state tax calculation
  printf ("Enter income from US bonds (was included in 'income' earlier)\n");
  scanf ("%lf", &usbondincome);
  reductions = ssreduce + maxcharity + usbondincome;
  printf ("CO reductions=$%.3lf\n", reductions);
  COincome   = taxable_income - reductions;
  COtax      = COincome*0.01*COtaxrate[idx] - tabor[idx];;
  printf ("CO tax=$%.3lfK\n", COtax);
	  
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

int get_topbracketidx (int idx, double taxable_income)
{
  for (int i=1; i<NUMBRACKETS; ++i) {
    if (taxable_income <= kicksinat[idx][i])
      return i-1;
  }
  return -1;
}
