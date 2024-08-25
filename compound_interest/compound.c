#include <stdio.h>
#include <math.h>

void get_doubling (double, int);
void get_amount (double, double, double, int);
void get_rate (double, double, double, int);
void get_years (double, double, double, int);

// Apply compound interest formula to solve for various terms in formula, at:
// www.thecalculatorsite.com/finance/calculators/compound-interest-formula
int main ()
{
  double A;  // Final amount
  double P;  // Principle
  double r;  // interest rate (as fraction)
  double t;  // number of time periods elapsed
  int n;     // number of times rate applied per time period
  int ans;   // what to compute

  while (1) {
    printf ("Enter 0 to compute doubling time given input of rate\n");
    printf ("Enter 1 to compute final amount given input of principle, rate, years\n");
    printf ("Enter 2 to compute rate needed to achieve input of principle, years, final amount\n");
    printf ("Enter 3 to compute years needed to achieve input final amount, rate\n");
    printf ("Enter -1 when done\n");
    scanf ("%d", &ans);

    if (ans > 0) {
      printf ("Enter principle\n");
      scanf ("%lf", &P);
    }

    switch (ans) {
    case -1:
      return 0;
    case 0:
      printf ("Enter rate (percent), compounding freq per year\n");
      scanf ("%lf %d", &r, &n);
      r *= 0.01;
      get_doubling (r, n);
      break;
    case 1:
      printf ("Enter rate (percent), years, compounding freq per year \n");
      scanf ("%lf %lf %d", &r, &t, &n);
      r *= 0.01;
      get_amount (P, r, t, n);
      break;
    case 2:
      printf ("Enter years, final amount, compounding freq per year\n");
      scanf ("%lf %lf %d", &t, &A, &n);
      get_rate (P, t, A, n);
      break;
    case 3:
      printf ("Enter final amount, rate (percent), compounding freq per year\n");
      scanf ("%lf %lf %d", &A, &r, &n);
      r *= 0.01;
      get_years (P, A, r, n);
      break;
    default:
      printf ("%d is not valid. Please enter a value between -1 and 3\n", ans);
    }
    printf ("\n");
  }
}

void get_doubling (double r, int n)
{
  double t = log (2.) / (n*log (1. + r/n));
  printf ("rate=%.1lf%% cf=%d -> doubling time=%.1lf years\n", r*100., n, t);
}

void get_amount (double P, double r, double t, int n)
{
  double A = P*pow((1. + r/n), n*t);
  printf ("Principle=%.1lf rate=%.1lf%% years=%.1lf cf=%d -> final amount=%.2lf\n",
	  P, r*100., t, n, A);
}

void get_rate (double P, double t, double A, int n)
{
  double r = n*(exp (log (A/P)/(n*t)) - 1.); // 100 is to convert fraction to percent
  printf ("Principle=%.1lf years=%.1lf final amount=%.1lf cf=%d -> required rate=%.1f%%\n",
	  P, t, A, n, r*100.);
}

void get_years (double P, double A, double r, int n)
{
  double t = log (A/P) / (n*log (1. + r/n));
  printf ("Principle=%.1lf final amount=%.1lf rate=%.1lf%% cf=%d -> will take %.1lf years\n",
	  P, A, r*100., n, t);
}
