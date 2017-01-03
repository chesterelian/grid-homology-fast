#ifndef POINT_H
#define POINT_H

struct Point {
  int x;
  int y;
};

bool operator<(const Point &lhs, const Point &rhs) {
  return lhs.x < rhs.x && lhs.y < rhs.y;
}

int eye(const Point a[], const Point b[]) {
  int counter = 0;
  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++)
      if (a[i] < b[j])
        counter++;
  return counter;
}

#endif
