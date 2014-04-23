void sub_ (int custom, int veclen, float *x, float *y, float *z, int niter)
{
  int i, iter;

  __declspec (vector) extern float slowpowf_ (float *, float *);
  
  for (iter = 0; iter < niter; ++iter) {
#pragma ivdep
    for (i = 0; i < veclen; ++i) {
      if (custom)
	z[i] = powf (x[i],y[i]);
      else
	z[i] = slowpowf_ (&x[i],&y[i]);
    }
  }
  return;
}
