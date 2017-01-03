#ifndef UTIL_H
#define UTIL_H

namespace util {

// assumption: 0 <= n <= 12
int factorial(int n);

// assumption: a < c
// returns true iff a < b < c
bool isBetween(int a, int b, int c);

// assumption: a < c
// returns true iff b < a or b > c
bool isntBetween(int a, int b, int c);

unsigned long long chooseBig(unsigned long long, unsigned long long);
int choose(int, int);
int choose2(int);
int z2n(int);
int n2z(int);

void swap(int &, int &);
void swap(bool &, bool &);

// -------------------------------------------------------------- implementation

// stop at 12! because 13! > INT_MAX
const int factorial_[] = {
  1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880, 3628800, 39916800, 479001600
};
int factorial(int n) { return factorial_[n]; }

bool isBetween(int a, int b, int c) { return a < b && b < c; }

bool isntBetween(int a, int b, int c) { return b < a || b > c; }

const int chooseMax_ = 30; // must be even
int choose_[chooseMax_ / 2 + 1][chooseMax_ / 2 + 1];
bool isChooseInit = false;
void initChoose() {
  int i, j;
  choose_[0][0] = 1;
  for (i = 1; i <= chooseMax_ / 2; i++) choose_[0][i] = choose_[i][0] = 1;
  for (i = 1; i <= chooseMax_ / 2; i++)
    for (j = i; j <= chooseMax_ / 2; j++)
      choose_[i][j] = choose_[i - 1][j] + (j == i ? choose_[i - 1][j] : choose_[i][j - 1]);
  for (j = 1; j < chooseMax_ / 2; j++)
    choose_[chooseMax_ / 2][j] = choose_[chooseMax_ / 2][j - 1] + choose_[j][chooseMax_ / 2];
  for (j = 1; j < chooseMax_ / 2 - 1; j++)
    for (i = chooseMax_ / 2 - 1; i > j; i--)
      choose_[i][j] = choose_[i][j - 1] + choose_[i + 1][j];
  isChooseInit = true;
}

int choose(int n, int k) {
  if (!isChooseInit) initChoose();
  if (k > n - k) k = n - k;
  if (n <= chooseMax_ / 2) return choose_[k][n - k];
  if (n - k <= chooseMax_ / 2) return choose_[k][n - k];
  return choose_[chooseMax_ + 1 - n + k][k];
}

int choose2(int n) { return n * (n - 1) / 2; }

int z2n(int z) { return z >= 0 ? 2 * z : -2 * z - 1; }

int n2z(int n) { return n % 2 ? -(n + 1) / 2 : n / 2; }

void swap(int &a, int &b) { int c = a; a = b; b = c; }
void swap(bool &a, bool &b) { bool c = a; a = b; b = c; }

const int cMax_ = 70; // must be even
unsigned long long c_[cMax_ / 2 + 1][cMax_ / 2 + 1];
bool isCInit = false;
void initC() {
  int i, j;
  c_[0][0] = 1;
  for (i = 1; i <= cMax_ / 2; i++) c_[0][i] = c_[i][0] = 1;
  for (i = 1; i <= cMax_ / 2; i++)
    for (j = i; j <= cMax_ / 2; j++)
      c_[i][j] = c_[i - 1][j] + (j == i ? c_[i - 1][j] : c_[i][j - 1]);
  for (j = 1; j < cMax_ / 2; j++)
    c_[cMax_ / 2][j] = c_[cMax_ / 2][j - 1] + c_[j][cMax_ / 2];
  for (j = 1; j < cMax_ / 2 - 1; j++)
    for (i = cMax_ / 2 - 1; i > j; i--)
      c_[i][j] = c_[i][j - 1] + c_[i + 1][j];
  isCInit = true;
}

unsigned long long chooseBig(unsigned long long n, unsigned long long k) {
  if (!isCInit) initC();
  if (k > n - k) k = n - k;
  if (n <= cMax_ / 2) return c_[k][n - k];
  if (n - k <= cMax_ / 2) return c_[k][n - k];
  return c_[cMax_ + 1 - n + k][k];
}

}

#endif
