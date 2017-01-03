#ifndef LINALG_H
#define LINALG_H

#include "util.h"

namespace linalg {

int rank(bool** matrix, int rows, int cols) {
  int i, j, k, row;
  int r = 0; // rank
  int col = 0; // column currently being inspected
  bool foundPivot; // found pivot in col?
  for (row = 0; row < rows; row++) {
    foundPivot = false;
    while (!foundPivot && col < cols) {
      for (i = row; i < rows; i++) {
        if (matrix[i][col]) {
          foundPivot = true;
          r++;
          for (j = i + 1; j < rows; j++)
            if (matrix[j][col])
              for (k = col; k < cols; k++)
                matrix[j][k] = (matrix[j][k] != matrix[i][k]);
          if (row != i)
            for (k = 0; k < cols; k++)
              util::swap(matrix[row][k], matrix[i][k]);
          break;
        }
      }
      col++;
    }
  }
  return r;
}

}

#endif
