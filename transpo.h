#ifndef TRANSPO_H
#define TRANSPO_H

#include <cmath> // for sqrt

#include "point.h"
#include "util.h"

Point tid2transpo(int tid) {
  int y = static_cast<int>(floor((sqrt(8.0 * tid + 1) - 1) / 2)) + 1;
  int x = tid - y * (y - 1) / 2;
  if (tid < N * (N - 1) / 2) {
    Point transpo = {x, y};
    return transpo;
  } else {
    Point transpo = {y, x};
    return transpo;
  }
}

int transpo2tid(Point transpo) {
  if (transpo.x < transpo.y)
    return transpo.y * (transpo.y - 1) / 2 + transpo.x;
  else {
    Point temp = {transpo.y, transpo.x};
    return transpo2tid(temp) + N * (N - 1) / 2;
  }
}

void transpo2state(int state[], Point transpo, int newState[]) {
  for (int i = 0; i < N; i++) newState[i] = state[i];
  util::swap(newState[transpo.x], newState[transpo.y]);
}

int transpo2state(int state[], Point transpo) {
  int newState[N];
  transpo2state(state, transpo, newState);
  return state2sid(newState);
}

int transpo2state(int sid, int tid) {
  int state[N];
  sid2state(sid, state);
  return transpo2state(state, tid2transpo(tid));
}

#endif
