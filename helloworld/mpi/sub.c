#include <math.h>

double sumsub (int iam, long looplen)
{
    long i;
    double sum = 0.;

    for (i = 0; i < iam * looplen; ++i) {
	sum += i;
    }
    return sum;
}

double sinsub (int iam, long looplen)
{
    long i;
    double sum = 0.;

    for (i = 0; i < iam * looplen; ++i) {
	sum += sin ((double) i);
    }
    return sum;
}

double expsub (int iam, long looplen)
{
    long i;
    double sum = 0.;

    for (i = 0; i < iam * looplen; ++i) {
	sum += exp (1./(1. + i));
    }
    return sum;
}

double logsub (int iam, long looplen)
{
    long i;
    double sum = 0.;

    for (i = 0; i < iam * looplen; ++i) {
	sum += log (1. + i);
    }
    return sum;
}

double sqrtsub (int iam, long looplen)
{
    long i;
    double sum = 0.;

    for (i = 0; i < iam * looplen; ++i) {
	sum += sqrt ((double) i);
    }
    return sum;
}
