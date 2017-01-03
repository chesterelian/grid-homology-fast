#ifndef HOMOLOGY_H
#define HOMOLOGY_H

#include <algorithm>
#include "monom.h"

int chainLength(int m, int a);
int padding(int m, int a, int deg);

int chainLength(int m, int a) { return min((mMax - m) / 2, aMax - a); }

int padding(int m, int a, int deg) {
  if (deg == 0) return 0;
  int result = dimension[z2n(m)][z2n(a)];
  for (int i = 1; i <= deg; i++)
  dimension
}

#endif
