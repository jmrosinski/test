#include <stdio.h>
#include <math.h>

// Purpose: Estimate fed and state taxes owed for a given year. Also calculate size of check to
//   write to fed and state, given how much has been withheld.
// Though the code asks for user input, it's easiest to put these entries into a file, then run
//   ./.estimate < [input_file]
// User-supplied input file consistently requires floating point thousands of $
// Assumptions:
//   User takes standard deduction rather than itemizing.
//   Age 65+ is embedded in std_deduction[]. For other ages, just adjust this percentage
//   Charitable deductions apply only to state income tax. 
//   AMT doesn't apply. If it does apply, additional code will have to be written to account for it
//   State is Colorado. For others, rewrite code after the comment // Next: CO state tax calculation
//   Tax years 2023-2025 are supported. Adding a year just requires increasing NUMYRS and filling
//     idx_map[], kicksinat[][] and std_deduction[] entries.
//   Top fed bracket supported is kicksinat[][NUMBRACKETS-2]. The value for [NUMBRACKETS-1] exists
//     in order to show how far you are from hitting the next higher bracket.
//   Cap gains and qualified deductions rates are assumed to be 15%. If this is incorrect for
//     a given user, additional code must be written to account for it (yuck).
//   In 2025 a "bonus" to the standard deduction was added, but phases out above certain income
//     levels. It is ASSUMED that the user doesn't qualify for the bonus. Code would have to be
//     written to account for the phaseout (yuck).
//   Only taxable income (including social security), short and long cap gains, ordinary and
//     qualified dividends, US bond income, charity contributions, and qualified rebates are
//     considered. If other significant tax issues apply, code must be added to account for it

#define NUMYRS 3
#define NUMBRACKETS 6
const int idx_map[NUMYRS] = {2023,2024,2025}; // supported tax years
// kicksinat[][] is the income (thousands of $) at which the bracket starts  
const double kicksinat[NUMYRS][NUMBRACKETS] = {{0.,11., 44.725, 95.375, 182.1  ,231.521},
					       {0.,11.6,47.15, 100.525, 191.95 ,243.726},
					       {0.,11.9,48.475,103.350, 197.301,250.526}};
int main()
{
  // All dollar-based floating point settings and inputs are in thousands
  // std_deduction numbers assumes income exceeds threshhold thus the bonus $6K doesn't apply
  const double std_deduction[NUMYRS] = {15.7, 16.55, 17.75};           // Assumes age 65+
  const double taxrate [NUMBRACKETS] = {10., 12., 22., 24., 32., 35.}; // tax rate (%) for each bracket
  const double cgfrac = .15;    // Cap gains rate (fraction)
  const double ssfrac = .85;    // Fraction of social security that is taxed
  const double cg_losslim = 3.; // IRS limits net capital loss to $3K
  int i;                 // loop index needs to be saved
  int idx;               // user input: index into arrays to match "year"
  int year;              // user input: year to estimate federal taxes
  int topbracketidx;     // index of top income bracket
  double income;         // user input: taxable income (not including US bond income)
  double ira;            // IRA distributions total (just for comparison to income)
  double usbondincome;   // income from USbonds. NOTE: CO doesn't tax US bond income
  double shortcg;        // user input: short-term capital gains
  double longcg;         // user input: long-term capital gains
  double capgains;       // capital gains net
  double carryover = 0.; // extent to which net capital LOSS exceeds $3K
  double odiv;           // user input: ordinary dividends
  double qdiv;           // user input: qualified dividends
  double unqdiv;         // odiv - qdiv (unqualified dividends)
  double capg_qdiv;      // capital gains + qualified dividends (assumed taxed at cgfrac)
  double agi;            // adjusted gross income (income + ordinary dividends + cap gains)
  double taxable_income; // agi - std deduction - capg_qdiv
  double tax;            // estimated federal tax
  double cgtax;          // tax on cap gains+qdiv
  double ssincome;       // income from social security
  double fedrebate;      // fed deduction from tax (e.g. heat pump)
  double fedwh;          // fed withholding
  // CO-specific items
  const double COtaxrate [NUMYRS] = {4.4, 4.25, 4.4}; // CO tax rate (%) for each year
  double ssreduce;               // Age 65 and above CO doesn't tax ss income
  const double charity_adj = .5; // CO reduces charitable contributions by this amount (thousand $)
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

  printf ("Enter IRA distributions (subset of taxable income)\n");
  if (scanf ("%lf %*s", &ira) < 1 || ira < 0. || ira > income) {
    printf ("income not found, negative, or exceeds total income. Quitting\n");
    return -1;
  }
  printf ("Got ira distributions=$%.3lfK\n", ira);

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
  
  if (shortcg >= 0. && longcg >= 0.) {
    income  += shortcg;
    capgains = longcg;
    printf ("shortcg and longcg are both gains: shortcg=income, longcg taxed at cgrate=%d%%\n",
	    (int) (cgfrac*100.));
  } else {                    // at least one of shortcg, longcg is negative
    capgains = shortcg + longcg;
    if (capgains >= 0.) {     // one of shortcg, longcg is negative, the other non-negative
      printf ("Net capgains non-negative due to shortcg=$%.3lfK, longcg=$%.3lfK\n", shortcg, longcg);
      if (shortcg > longcg) { // shortcg is non-negative, longcg negative, so add the net to income
	income += capgains;
	// Zero capgains here: It's been added to income so it shouldn't be included in the
	// capg_qdiv calculation below this bunch of nested "if" stuff
	capgains = 0.;
	printf ("    income increased to $%.3lfK\n", income);
      } else {                // longcg is the positive one so leave as is and tax the net at cgrate
	printf ("    Net capgains=$%.3lfK\n", capgains);
      }
    } else { // capgains < 0:Doesn't matter whether both or just one of shortcg, longcg are negative
      printf ("cgloss=$%.3lfK due to shortcg=$%.3lfK longcg=$%.3lfK\n",
	      capgains, shortcg, longcg);
      carryover = fabs (fmin (0., capgains + cg_losslim)); // make "carryover" a non-negative number
      capgains  = fmax (capgains, -cg_losslim);            // negative net gains cannont exceed $3K
      printf ("  $%.3lfK will be treated as a loss and subtracted from income\n", capgains);
      printf ("  $%.3lfK will be carried over to the next year\n", carryover);
    }
  }

  // qdiv must be a positive number, but it's possible capgains+qdiv is negative. Thus fmax()
  capg_qdiv = fmax (0., capgains + qdiv);
  printf ("cap gains + qualified dividends=$%.3lfK ASSUMED to be federally taxed at %d%%\n",
	  capg_qdiv, (int) (cgfrac*100.));

  unqdiv = odiv - qdiv;
  printf ("unqualified dividends=$%.3lfK federally taxed as regular income\n", unqdiv);
  printf ("IRA ($%.3lfK) portion of income to be taxed off rate table ($%.3lfK) is %.1lf%%\n",
	  ira, income+unqdiv, 100.*(ira/(income+unqdiv)));
  
  agi = income + odiv + capgains;
  printf ("adjusted gross income=$%.3lfK\n", agi);
  printf ("capgains+qdiv (%.3lfK) portion of AGI is %.1lf%%\n", capg_qdiv, 100.*(capg_qdiv/agi));

  // NOTE taxable income INCLUDES unqdiv (unqualified dividends)
  taxable_income = agi - std_deduction[idx] - capg_qdiv;
  printf ("taxable income, subtracting std deduction ($%.3lfK) and cg+qdiv($%.3lfK)=$%.3lfK\n",
	  std_deduction[idx], capg_qdiv, taxable_income);

  if ((topbracketidx = get_topbracketidx (idx, taxable_income)) < 0) {
    printf ("Cannot find top tax bracket index. Quitting\n");
    return -1;
  }
  printf ("Your top tax bracket (%.0lf%%) begins at $%.3lfK\n",
	  taxrate[topbracketidx], kicksinat[idx][topbracketidx]);

  if (taxable_income > kicksinat[idx][NUMBRACKETS-1]) {
    printf ("Insufficient brackets:taxable_income=$%.3lfK exceeds top limit of $%.3lfK Quitting\n",
	    taxable_income, kicksinat[idx][NUMBRACKETS-1]);
    return -1;
  }

  // Loop over tax brackets to figure total tax
  tax = 0.;
  for (i = 0; i < NUMBRACKETS-1 && taxable_income >= kicksinat[idx][i]; ++i) {
    double limit = fmin (kicksinat[idx][i+1], taxable_income);
    tax += (limit - kicksinat[idx][i]) * 0.01*taxrate[i];  // 0.01 converts percent to fraction
    if (taxable_income < kicksinat[idx][i+1])
      break;
  }
  printf ("taxable_income=$%.3lfK is %.3lfK from bumping into the next bracket of %.0lf%%\n",
	  taxable_income, kicksinat[idx][i+1] - taxable_income, taxrate[i+1]);
	  
  printf ("tax on pure income=$%.3lfK\n", tax);
  // Add tax on capital gains+qualified dividends
  cgtax = capg_qdiv*cgfrac;
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
