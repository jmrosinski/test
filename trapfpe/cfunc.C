extern "C" {
float cfunc_ (float *x, float *y)
{
  return ((*x) / (*y));
}

int icfunc_div_ (int *x, int *y)
{
  return ((*x) / (*y));
}

int icfunc_mult_ (int *x, int *y)
{
  return ((*x) * (*y));
}
}
