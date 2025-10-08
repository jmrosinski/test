#include <stdio.h>
#include <math.h>

#define NUMYRS 3
#define NUMBRACKETS 5
const int idx_map[NUMYRS] = {2023,2024,2025}; // supported tax years
const double kicksinat[NUMYRS][NUMBRACKETS] = {{0.,11., 44.725, 95.375, 182.1  },
					       {0.,11.6,47.15, 100.525, 191.95 },
					       {0.,11.9,48.475,103.350, 197.300}};
int main()
{
  // All dollar-based floating point settings and inputs are in thousands
  const double std_deduction[NUMYRS] = {15.7, 16.55, 17.00};
  const double taxrate [NUMBRACKETS] = {10., 12., 22., 24., 32.}; // tax rate (%) for each bracket
  const double cgrate = 15.; // Cap gains rate ASSUMED to be this percent
  const double ssfrac = .85; // Fraction of social security that is taxed
  int idx;               // user input: index into arrays to match "year"
  int year;              // user input: year to estimate federal taxes
  int topbracketidx;     // index of top income bracket
  double income;         // user input: taxable income (not including US bond income)
  double usbondincome;   // income from USbonds. NOTE: CO doesn't tax US bond income
  double shortcg;        // user input: short-term capital gains
  double longcg;         // user input: long-term capital gains
  double capgains;       // capital gains net
  double carryover = 0.; // extent to which net capital LOSS exceeds $3K
  double odiv;           // user input: ordinary dividends
  double qdiv;           // user input: qualified dividends
  double unqdiv;         // odiv - qdiv (unqualified dividends)
  double capg_qdiv;      // capital gains + qualified dividends (assumed taxed at cgrate)
  double agi;            // adjusted gross income (income + ordinary dividends + cap gains)
  double taxable_income; // agi - std deduction - capg_qdiv
  double tax;            // estimated federal tax
  double cgtax;          // tax on cap gains+qdiv
  double ssincome;       // income from social security
  double fedrebate;      // fed deduction from tax (e.g. heat pump)
  double fedwh;          // fed withholding
  // CO-specific items
  const double COtaxrate [NUMYRS] = {4.4, 4.25, 4.25}; // CO tax rate (%) for each year
  double ssreduce;               // Age 65 and above CO doesn't tax ss income
  const double charity_adj = .5; // CO reduces charitable contributions by this amount
  double charity;                // CO allows deductions for charitable contributions
  double COwh;                   // CO withholding
  const double tabor[NUMYRS] = {0.8, 0.323, 0.}; // 0.323 is for AGI between $166K and $233K
  double reductions;             // reductions to income for CO tax purposes
  double COincome;               // income to apply CO tax rate to
  double COtax;                  // CO tax owed

  // function prototypes
  int get_yridx (int);                 // find index to match year
  int get_topbracketidx (int, double); // find index of top tax bracket

  printf ("Enter tax year for calculation:\n");
  if (scanf ("%d %*s", &year) < 1) {
    printf ("Year not found. Quitting\n");
    return -1;
  }
  if ((idx = get_yridx (year)) < 0) {
    printf ("index for year=%d not found. Quitting\n", year);
    return -1;
  }
  printf ("Estimating federal tax for year=%d\n", year);
  printf ("NOTE: All requested input is expected in floating point thousands of dollars\n");
  
  printf ("Enter taxable income exclude ss, capgains, dividends and USbonds (maybe 4b+5b of 1040)\n");
  if (scanf ("%lf %*s", &income) < 1 || income < 0.) {
    printf ("income not found or negative. Quitting\n");
    return -1;
  }
  printf ("Got income=$%.3lfK\n", income);

  printf ("Enter total social security income\n");
  if (scanf ("%lf %*s", &ssincome) < 1 || ssincome < 0.) {
    printf ("social security income not found or negative. Quitting\n");
    return -1;
  }
  printf ("Got social security income=$%.3lfK\n", ssincome);

  printf ("Enter income from US bonds ()\n");
  if (scanf ("%lf %*s", &usbondincome) < 1 || usbondincome < 0.) {
    printf ("income from US bonds not found or negative. Quitting\n");
    return -1;
  }
  printf ("Got US bond income=$%.3lfK\n", usbondincome);
  income += ssfrac*ssincome + usbondincome;

  printf ("Enter ordinary dividends (maybe line 3b of 1040)\n");
  if (scanf ("%lf %*s", &odiv) < 1 || odiv < 0.) {
    printf ("ordinary dividend income not found or negative. Quitting\n");
    return -1;
  }
  printf ("Got ordinary dividend income=$%.3lfK\n", odiv);

  printf ("Enter qualified dividends (maybe line 3a of 1040)\n");
  if (scanf ("%lf %*s", &qdiv) < 1 || qdiv < 0.) {
    printf ("qualified dividend income not found or negative. Quitting\n");
    return -1;
  }
  printf ("Got qualified dividend income=$%.3lfK\n", qdiv);

  printf ("Enter short-term capital gains (maybe line 7 of schedule D)\n");
  if (scanf ("%lf %*s", &shortcg) < 1) {
    printf ("short term capital gains not found. Quitting\n");
    return -1;
  }
  printf ("Got short-term capital gains=$%.3lfK\n", shortcg);
    
  printf ("Enter long-term capital gains (maybe line 15 of schedule D)\n");
  if (scanf ("%lf %*s", &longcg) < 1) {
    printf ("long term capital gains not found. Quitting\n");
    return -1;
  }
  printf ("Got long-term capital gains=$%.3lfK\n\n", longcg);

  printf ("Enter charitable contributions (for now this only applies to CO tax)\n");
  if (scanf ("%lf %*s", &charity) < 1) {
    printf ("charity not found. Quitting\n");
    return -1;
  }
  printf ("Got charitable contributions=$%.3lfK\n\n", charity);

  printf ("Enter federal rebates total (to be subtracted from tax owed)\n");
  if (scanf ("%lf %*s", &fedrebate) < 1) {
    printf ("fed rebate not found. Quitting\n");
    return -1;
  }
  printf ("Got fed rebates=$%.3lfK\n\n", fedrebate);

  printf ("Enter federal withholding\n");
  if (scanf ("%lf %*s", &fedwh) < 1) {
    printf ("federal withholding not found. Quitting\n");
    return -1;
  }
  printf ("Got federal withholding=$%.3lfK\n\n", fedwh);

  printf ("Enter CO withholding\n");
  if (scanf ("%lf %*s", &COwh) < 1) {
    printf ("CO withholding not found. Quitting\n");
    return -1;
  }
  printf ("Got CO withholding=$%.3lfK\n\n", COwh);

  // Figure rate on capital gains. Some fraction of short-term gains will be taxed as regular
  // income if net gains (short+long) are positive. If both shortcg and longcg are positive,
  // ALL of shortcg will be taxed as regular income, and ALL of longcg will be taxed at cgrate.
  // If net cap losses exceed $3K, only the $3K will be counted this year and carried over to the
  // next year.
  
  if (shortcg > 0. && longcg > 0.) {
    income  += shortcg;
    capgains = longcg;
    printf ("shortcg and longcg are both gains: shortcg=income, longcg taxed at cgrate=%d%%\n",
	    (int) cgrate);
  } else {                    // at least one of, maybe both of shortcg, longcg are negative
    capgains = shortcg + longcg;
    if (capgains > 0.) {      // one of shortcg, longcg is positive, the other negative
      printf ("Net capgains positive due to shortcg=$%.3lfK, longcg=$%.3lfK\n", shortcg, longcg);
      if (shortcg > longcg) { // shortcg is the positive one so add the net to income
	income += capgains;
	// I THINK it's right to zero capgains here: It's been added to income so it shouldn't be
	// included in the capg_qdiv calculation below this bunch of nested "if" stuff
	capgains = 0.;
	printf ("    income increased to $%.3lfK\n", income);
      } else {                // longcg is the positive one so leave as is and tax the net at cgrate
	printf ("    Net capgains=$%.3lfK\n", capgains);
      }
    } else { // capgains < 0:Doesn't matter whether both or just one of shortcg, longcg are negative
      printf ("cgloss=$%.3lfK due to shortcg=$%.3lfK longcg=$%.3lfK\n",
	      capgains, shortcg, longcg);
      carryover = fabs (fmin (0., capgains + 3.));  // make "carryover" a non-negative number
      capgains  = fmax (capgains, -3.);             // negative net gains cannont exceed $3K
      printf ("  $%.3lfK will be treated as a loss and subtracted from income\n", capgains);
      printf ("  $%.3lfK will be carried over to the next year\n", carryover);
    }
  }

  // qdiv must be a positive number, but it's possible capgains+qdiv is negative. Thus fmax()
  capg_qdiv = fmax (0., capgains + qdiv);
  printf ("cap gains + qualified dividends=$%.3lfK ASSUMED to be federally taxed at %d%%\n",
	  capg_qdiv, (int) cgrate);

  unqdiv = odiv - qdiv;
  printf ("unqualified dividends=$%.3lfK federally taxed as regular income\n", unqdiv);
  
  agi = income + odiv + capgains;
  printf ("adjusted gross income=$%.3lfK\n", agi);
  
  taxable_income = agi - std_deduction[idx] - capg_qdiv;
  printf ("taxable income (subtracting cg and qdiv)=$%.3lfK\n", taxable_income);

  if ((topbracketidx = get_topbracketidx (idx, taxable_income)) < 0) {
    printf ("Cannot find top tax bracket index. Quitting\n");
    return -1;
  }
  printf ("Top tax bracket begins at $%.3lfK\n", kicksinat[idx][topbracketidx]);

  if (taxable_income > kicksinat[idx][NUMBRACKETS-1]) {
    printf ("Insufficient brackets:taxable_income=$%.3lfK exceeds top limit of $%.3lfK Quitting\n",
	    taxable_income, kicksinat[idx][NUMBRACKETS-1]);
    return -1;
  }

  // Loop over tax brackets to figure total tax
  tax = 0.;
  for (int i = 0; i < NUMBRACKETS-1 && taxable_income >= kicksinat[idx][i]; ++i) {
    double limit = fmin (kicksinat[idx][i+1], taxable_income);
    tax += (limit - kicksinat[idx][i]) * 0.01*taxrate[i];  // 0.01 converts percent to fraction
    if (taxable_income < kicksinat[idx][i+1])
      break;
  }
  printf ("tax on pure income=$%.3lfK\n", tax);
  // Add tax on capital gains+qualified dividends
  cgtax = capg_qdiv*0.01*cgrate;
  printf ("tax on cap gains+qdiv=$%.3lfK\n", cgtax);
  tax += cgtax;
  printf ("federal tax=$%.3lfK\n", tax);
  tax -= fedrebate;
  printf ("federal tax after rebates=$%.3lfK\n", tax);
  printf ("federal check to write (ie after withholding)=$%.3lfK\n\n", tax-fedwh);

  // Next: CO state tax calculation
  ssreduce   = ssfrac*ssincome;
  charity   -= fmin (charity - charity_adj, 0.); // CO reduces charity contribs by some amount
  reductions = ssreduce + charity + usbondincome;
  printf ("CO reductions to income=$%.3lfK\n", reductions);
  // CO taxes cap gains and ALL dividends (not just unqualified dividends) as regular income
  // NOTE taxable_income already INCLUDES unqualified dividends (unqdiv)
  COincome   = taxable_income + capg_qdiv - reductions;
  printf ("CO taxable income=$%.3lfK\n", COincome);
  COtax      = COincome*0.01*COtaxrate[idx];
  printf ("CO tax=$%.3lfK\n", COtax);
  COtax -= tabor[idx];
  printf ("CO tax after Tabor adjustment=$%.3lfK\n", COtax);
  printf ("CO check to write (ie after withholding)=$%.3lfK\n", COtax-COwh);
  
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
