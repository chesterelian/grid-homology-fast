#ifndef STATE_H
#define STATE_H

#include <iostream>
#include "point.h"

using namespace std;

void sid2state(int sid, int state[]) {
  int els[N];
  int j;
  for (int i = 0; i < N; i++) els[i] = i;
  for (int i = 0; i < N; i++) {
    j = sid % (N - i);
    sid /= N - i;
    state[i] = els[j] + 1;
    els[j] = els[N - i - 1];
  }
}

int state2sid(const int state[]) {
  int perm[N];
  int pos[N];
  int els[N];
  for (int i = 0; i < N; i++) {
    perm[i] = state[i] - 1;
    pos[i] = i;
    els[i] = i;
  }
  int k = 0;
  int m = 1;
  for (int i = 0; i < N - 1; i++) {
    k += m * pos[perm[i]];
    m *= N - i;
    pos[els[N - i - 1]] = pos[perm[i]];
    els[pos[perm[i]]] = els[N - i - 1];
  }
  return k;
}

void state2points(const int state[], Point sPoints[]) {
  for (int i = 0; i < N; i++) {
    sPoints[i].x = i * scaleFactor;
    sPoints[i].y = (state[i] - 1) * scaleFactor;
  }
}

void printState(const int state[]) {
  cout << "[" << state[0];
  for (int i = 1; i < N; i++) cout << "," << state[i];
  cout << "]";
}

void printState(int sid) {
  int state[N];
  sid2state(sid, state);
  printState(state);
}

void asciiState(const int state[]) {
  cout << "[";
  for (int i = 0; i < N; i++) cout << static_cast<char>(state[i] + '0');
  cout << "]";
}

void asciiState(int sid) {
  int state[N];
  sid2state(sid, state);
  asciiState(state);
}

#endif
