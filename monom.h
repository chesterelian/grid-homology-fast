#ifndef MONOMIAL_H
#define MONOMIAL_H

#include "util.h"
using namespace util;

void listMonoms(int n, int d, int** monoms);
int monom2mid(int monom[]);
int monomCmp(int lhs[], int rhs[]); // returns 1 if lhs > rhs, 0 if ==, -1 if <
int numMonomsDeg(int deg); // number of monomials of degree deg
int numMonomsDegLe(int deg); // number of monomials of degree <= deg (including deg 0, which has 1 monomial)
int getDeg(const int monom[]);
void initMonoms();
void initMonoms(int deg);
void printMonom(int deg, int mid);

const int maxDeg = 10; // allocate space to initialize monomials up to degree 10
int** monoms[maxDeg + 1]; // slots 0 and 1 go to waste
bool isMonomInit[maxDeg + 1]; // slots 0 and 1 go to waste

// -------------------------------------------------------------- implementation

int getDeg(const int monom[]) {
  int deg = 0;
  for (int i = 0; i < N; i++) deg += monom[i];
  return deg;
}

void initMonoms() {
  for (int i = 0; i < maxDeg + 1; i++) isMonomInit[i] = false;
}

void initMonoms(int deg) {
  if (!isMonomInit[deg]) {
    int length = numMonomsDeg(deg);
    monoms[deg] = new int*[length];
    for (int i = 0; i < length; i++) monoms[deg][i] = new int[N];
    listMonoms(N, deg, monoms[deg]);
    isMonomInit[deg] = true;
  }
}

void printMonom(int deg, int mid) {
  cout << "(";
  if (deg == 1) {
    for (int i = 0; i < N; i++) cout << (i == mid ? 1 : 0);
  }
  else if (deg > 1) {
    initMonoms(deg);
    for (int i = 0; i < N; i++) cout << monoms[deg][mid][i];
  }
  cout << ")";
}

int numMonomsDeg(int deg) { return choose(N + deg - 1, deg); }
int numMonomsDegLe(int deg) { return choose(N + deg, deg + 1); }

/*
 * Think of it as trying to populate a (n+d-1 choose d) x (n) matrix.
 */
void listMonoms(int n, int d, int** monoms) {
  int rows = choose(n + d - 1, d);
  int pascal[d + 1]; // [10, 6, 3, 1]
  int pascalCum[d + 2]; // [0, 10, 16, 19, 20]
  pascalCum[0] = 0;
  for (int row = 0; row < rows; row++) monoms[row][0] = d;
  for (int col = 1; col < n; col++) {
    for (int i = 0; i <= d; i++) {
      pascal[i] = choose(d - i + col - 1, col - 1);
      pascalCum[i + 1] = pascalCum[i] + pascal[i];
    }
    for (int i = 0; i <= d; i++)
      for (int j = 0; j < pascal[i]; j++)
        monoms[pascalCum[i] + j][col] = i;
    for (int i = 1; i <= d; i++)
      for (int j = pascalCum[i]; j < pascalCum[i + 1]; j++)
        for (int k = col - 1; k >= 0; k--)
          monoms[j][k] = monoms[j - pascalCum[i + 1] + pascal[0]][k];
  }
  for (int i = 0; i < rows; i++)
    for (int j = 0; j <= n - 2; j++)
      monoms[i][j] -= monoms[i][j + 1];
}

int monom2mid(int monom[]) {
  int deg = getDeg(monom);
  if (deg == 1)
    for (int i = 0; i < N; i++)
      if (monom[i] == 1)
        return i;
  initMonoms(deg);
  // now do binary search
  int low = 0;
  int high = numMonomsDeg(deg) - 1;
  int cmp;
  while (low <= high) {
    int mid = (low + high) / 2;
    cmp = monomCmp(monom, monoms[deg][mid]);
    if (cmp == 0)
      return mid;
    else if (cmp == 1) // if my monom is bigger than my guess
      low = mid + 1;
    else
      high = mid - 1;
  }
  return -1;
}

int monomCmp(int lhs[], int rhs[]) {
  for (int i = N - 1; i >= 0; i--)
    if (lhs[i] != rhs[i])
      return lhs[i] < rhs[i] ? -1 : 1;
  return 0;
}

#endif
