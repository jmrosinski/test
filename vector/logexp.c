#include <math.h>
#include <stdio.h>

#if ( defined FORTRANUNDERSCORE )
#define slowexpf slowexpf_
#define slowlogf slowlogf_
#define slowpowf slowpowf_
#elif ( defined FORTRANDOUBLEUNDERSCORE )
#define slowexpf slowexpf_
#define slowlogf slowlogf_
#define slowpowf slowpowf_
#endif

// Build offload versions of slowexpf, slowlogf, and slowpowf
#ifdef TARGETMIC
__attribute__ ((target (mic))) float slowexpf (float *);
__attribute__ ((target (mic))) float slowlogf (float *);
__attribute__ ((target (mic))) float slowpowf (float *, float *);
#else
__declspec (vector) float slowexpf (float *);
__declspec (vector) float slowlogf (float *);
__declspec (vector) float slowpowf (float *, float *);
#endif

// Taylor series utilizing the relation exp(x) = base**k * exp(r) where x = base + k*ln(base)
// Found at: http://math.stackexchange.com/questions/18445/fastest-way-to-calculate-ex-upto-arbitrary-number-of-decimals
// Comuting exp(r) instead of exp(x) helps because Taylor series converges fastest for small arguments

__declspec (vector) float slowexpf (float *exponent)
{
#pragma offload_attribute (push, target(mic))
  static const float LN10 = 2.302585092994046f;  // ln(10)
  float locexp;       // x from exp(x), normalized to be between 0 and 1
  float sum;          // part of taylor series expansion
  float prvsum;       // previous iteration value of sum
  float frac;         // summation term for current iteration
  int i;              // loop index
  float tentothek;    // 10**k
  float result;       // exp(x)
  float r;            // shifted version of input argument "x"
  int k;              // "k" from algebraic expression above
  int neg = 0;        // logical: 1 means negative argument, 0 means non-negative
#pragma offload_attribute (pop)

  locexp = *exponent;

  // Return infinity if input too big for IEEE 32-bits, zero if too small.
  // 90 definitely exceeds the bounds. That's OK--the algorithm will 
  // also generate inf or 0 as appropriate for abs(arg) =~ 89 
  if (locexp > 90.)
    return (1./0.);
  else if (locexp < -90.)
    return 0.;

  // Negative exponent means compute exp(-x) = 1./exp(x)
  if (locexp < 0.) {
    neg = 1;
    locexp = -locexp;
  }

  // Adjust "x" in exp(x) for faster convergence using relation exp(x) = 10**k * exp(r) where x = r + k*ln(10)
  r = locexp;
  tentothek = 1.;
  for (k = 1; r > 1.; ++k) {
    r = locexp - k*LN10;
    tentothek *= 10.;
  } 

  // Apply Taylor series on "r" which is between 1-ln10 and 1
  frac = r;
  prvsum = 1.0;
  sum = (1.0 + frac);  // 1 + r from taylor series. 

  // Now the rest of the taylor series: loop until we're beyond the precision range
  for (i = 2; sum != prvsum; ++i) {
    frac *= r/i;       // current value of (x**n)/n!
    prvsum = sum;
    sum += frac;
  }

#ifdef DEBUG
  printf ("1st loop iterations=%d\n", k-1);
  printf ("2nd loop iterations=%d\n", i-2);
  printf ("tentothek=%e\n", tentothek);
#endif

  // Final result calculation
  result = tentothek * sum;
  if (neg)
    return 1./result;
  else
    return result;
}

// Adapted from http://en.wikipedia.org/wiki/Logarithm  See summation notation about halfway down the page
__declspec (vector) float slowlogf (float *x)
{
#pragma offload_attribute (push, target(mic))
  float ratio;   // ((z-1)/(z+1)) from wikipedia page 
  float ratiosq; // ratio*ratio 
  float ans;     // result before normalizing "z" 
  float term;    // ((z-1)/(z+1))**(2n+1) from wikipedia page 
  float z;       // number to take log of ("z" from wikipedia page) 
  int n;         // loop index over nterms 
  int iter = 0;  // number of iterations to get x near 1 
  int sign = 1;  // whether have to multiply or divide to get argument near 1 
  static const float LN10 = 2.302585092994046f;  // ln(10)
  static const int nterms = 10;  // this choice of number of terms to include seems to work well 
#pragma offload_attribute (pop)

  // log(0) is -infinity
  if (*x == 0.)
    return -(1./0.);

  // log of a negative number is NaN
  if (*x < 0.)
    return (0./0.);

  // shift argument for fast convergence
  z = *x;
  if (z > 5.) {
    for (iter = 0; z > 5.; ++iter) {
      z *= 0.1;
    }
  } else if (z < 0.2) {
    sign = -1;
    for (iter = 0; z < 0.2; ++iter) {
      z *= 10.;
    }
  }

  // apply formula from wikipedia page
  ratio = (z - 1.) / (z + 1.);
  ratiosq = ratio*ratio;
  term = ratio;
  ans = term;
  for (n = 1; n < nterms; ++n) {
    term *= ratiosq;
    ans += term/(2*n + 1);
  }
#ifdef DEBUG
  float biggest = ratio;
  float smallest = term / (2*(nterms-1)+1);
  printf ("biggest term=%f smallest term=%f\n", biggest, smallest);
#endif

  // result needs to be normalized if any shifting was done
  return 2.*ans + sign*iter*LN10;
}

// Power function: utilize y**x = exp(x*log(y))
__declspec (vector) float slowpowf (float *base, float *power)
{
#pragma offload_attribute (push, target(mic))
  float arg;
#pragma offload_attribute (pop)

  if (*base < 0.)
    return (0./0.);  // nan

  arg = fabsf (*power) * slowlogf (base);

  if (*power < 0.)
    return 1./slowexpf (&arg);
  else
    return slowexpf (&arg);
}
