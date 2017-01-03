#ifndef RECT_H
#define RECT_H

#include "point.h"
#include "util.h"
using namespace util;

namespace rect {

// returns true iff p is in the rectangle (sw, ne)
bool inRect(const Point sw, const Point ne, const Point p);

// returns true iff the rectangle (sw, ne) contains none of the points in
// points[]; length is the length of points[]
bool hasNone(const Point sw, const Point ne, const Point points[], int length);

// points[] must be oPoints or xPoints, so always has length N
// count[] is where the result is stored, so count also has length N
// count[i] is 1 or 0; it's 1 iff the rectangle (sw, ne) contains points[i]
void oxCount(const Point sw, const Point ne, const Point points[], int count[]);

// -------------------------------------------------------------- implementation

bool inRect(const Point sw, const Point ne, const Point p) {
  if (sw.x < ne.x) {
    if (sw.y < ne.y)
      return isBetween(sw.x, p.x, ne.x) && isBetween(sw.y, p.y, ne.y);
    else
      return isBetween(sw.x, p.x, ne.x) && isntBetween(ne.y, p.y, sw.y);
  } else {
    if (sw.y < ne.y)
      return isntBetween(ne.x, p.x, sw.x) && isBetween(sw.y, p.y, ne.y);
    else
      return isntBetween(ne.x, p.x, sw.x) && isntBetween(ne.y, p.y, sw.y);
  }
}

bool hasNone(const Point sw, const Point ne, const Point points[], int length) {
  for (int i = 0; i < length; i++)
    if (inRect(sw, ne, points[i]))
      return false;
  return true;
}

// points must be oPoints or xPoints
void oxCount(const Point sw, const Point ne, const Point points[], int count[]) {
  for (int i = 0; i < N; i++)
    count[i] = inRect(sw, ne, points[i]) ? 1 : 0;
}


}

#endif
