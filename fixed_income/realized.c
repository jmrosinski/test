#include <stdio.h>
#include <math.h>

int main ()
{
  double interest;   // interest received on fixed income
  double taxrate;    // top end tax rate (fed + maybe state)
  double inflation;  // inflation rate
  double actual;     // gain after taxes taken out
  double realized;   // gain after taxes AND inflation

  printf ("Enter interest rate on fixed asset\n");
  scanf ("%lf", &interest);

  printf ("Enter total tax rate as floating point percent\n");
  scanf ("%lf", &taxrate);

  printf ("Enter yearly inflation rate as floating point percent\n");
  scanf ("%lf", &inflation);

  interest *= 0.01;
  taxrate *= 0.01;
  inflation *= 0.01;

  actual = interest*(1. - taxrate);
  realized = actual - inflation;

  printf ("rate=%4.2lf%% tax=%4.2lf%% inflation=%4.2lf%% results in realized gain of %4.2lf%%\n",
	  100.*interest, 100.*taxrate, 100.*inflation, 100.*realized);;
  return 0;
}
