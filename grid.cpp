#include <iostream>
#include <algorithm> // for min

// ASCII: 0123456789:;<=>?@ABCDE...

const char ox[] = "2134 1423";
//const char ox[] = "56341287 84523761";
//const char ox[] = "123456789: 456789:123";
const int N = 4;
const int scaleFactor = 2; // must be multiple of 2
const int gradingBound = 20; // assume M and A are in the range [-gradingBound, gradingBound]
int dimension[gradingBound * 2 + 1][gradingBound * 2 + 1];
int mMin = gradingBound;
int aMin = gradingBound;
int mMax = -gradingBound;
int aMax = -gradingBound;

#include "util.h"
#include "point.h"
#include "state.h"
#include "transpo.h"
#include "rect.h"
#include "linalg.h"
#include "monom.h"
using namespace std;
using namespace util;
using namespace rect;

int* mGrading = new int[factorial(N)];
int* aGrading = new int[factorial(N)];
int* gid = new int[factorial(N)];

Point oPoints[N];
Point xPoints[N];

struct Generator {
  int deg; // degree of monomial
  int mid;
  int sid;
};

/*
Generator operator*(int v[], Generator g) {
  Generator r;
  r.sid = g.sid;
  for (int i = 0; i < N; i++) r.v[i] = g.v[i] + v[i];
  return r;
}
*/

//void diff(const Point oPoints[], const Point xPoints[], int sid, int result[][N]) {
void diff(int sid, int result[][N]) {
  int state[N];
  Point sPoints[N];
  sid2state(sid, state);
  state2points(state, sPoints);

  int coeff[N];
  for (int tid = 0; tid < choose2(N); tid++) {
    Point transpo = tid2transpo(tid);
    Point vertex1 = sPoints[transpo.x];
    Point vertex2 = sPoints[transpo.y];

    // rect1 = (vertex1, vertex2)
    if (hasNone(vertex1, vertex2, sPoints, N) && hasNone(vertex1, vertex2, xPoints, N)) {
      oxCount(vertex1, vertex2, oPoints, coeff);
      for (int i = 0; i < N; i++) result[tid][i] = coeff[i];
    } else {
      result[tid][0] = -1;
      //for (int i = 1; i < N; i++) result[tid][i] = 0;
    }
    // rect2 = (vertex2, vertex1)
    if (hasNone(vertex2, vertex1, sPoints, N) && hasNone(vertex2, vertex1, xPoints, N)) {
      oxCount(vertex2, vertex1, oPoints, coeff);
      for (int i = 0; i < N; i++) result[tid + choose2(N)][i] = coeff[i];
    } else {
      result[tid + choose2(N)][0] = -1;
      //for (int i = 1; i < N; i++) result[tid][i] = 0;
    }
  }
}

void initOxPoints();
void computeGradings();
void printGradings();
void printDimensions();

int main() {
  initOxPoints();
  computeGradings();
  printDimensions();

  // ------------------------------------------------ begin homology computation

  initMonoms();

  // grading whose homology we want to compute
  int mHom = -4;
  int aHom = -2;
  cout << "Computing homology of grading (" << mHom << "," << aHom << ")...\n";

  // dimension of domain, i.e., dimension of (mHom, aHom) (without the V's)
  int dimDom = dimension[z2n(mHom)][z2n(aHom)];

  // dimension of (mHom - 1, aHom) (with the V's)
  int dimCodom;

  int chainLength = min((mMax - (mHom - 1)) / 2, aMax - aHom);
  //cout << "chainLength is " << chainLength << endl;
  int* bookmarks = new int[chainLength + 1];
  bookmarks[0] = 0;

  dimCodom = 0;
  for (int i = 0; i <= chainLength; i++) {
    //cout << "dimCodom is now " << dimCodom << endl;
    dimCodom += numMonomsDeg(i) * dimension[z2n((mHom - 1) + i * 2)][z2n(aHom + i)];
    bookmarks[i + 1] = dimCodom;
  } // at this point dimCodom should be correct

  Generator* dom = new Generator[dimDom];
  Generator* codom = new Generator[dimCodom];
  bool** d = new bool*[dimCodom]; // boolean matrix
  for (int i = 0; i < dimCodom; i++) {
    d[i] = new bool[dimDom];
    for (int j = 0; j < dimDom; j++) d[i][j] = false;
  }
  cout << "Domain has dimension " << dimDom << endl;
  cout << "Codomain has dimension " << dimCodom << endl;

  for (int sid = 0; sid < factorial(N); sid++) {
    int mCur = mGrading[sid];
    int aCur = aGrading[sid];
    if (aCur == aHom) {
      if (mCur == mHom) { // in domain
        dom[gid[sid]].sid = sid;
        dom[gid[sid]].deg = 0;
        dom[gid[sid]].mid = 0;
      } else if (mCur == mHom - 1) { // in codomain (not up the chain)
        codom[gid[sid]].sid = sid;
        codom[gid[sid]].deg = 0;
        codom[gid[sid]].mid = 0;
      }
    } else if (aCur > aHom && 2*(aCur-aHom)-mCur+mHom == 1) { // in codomain (up the chain)
      int howFarUp = aCur - aHom;
      int numMonoms = numMonomsDeg(howFarUp);
      for (int mid = 0; mid < numMonoms; mid++) {
        int ind = bookmarks[howFarUp] + gid[sid] * numMonoms + mid;
        codom[ind].sid = sid;
        codom[ind].deg = howFarUp;
        codom[ind].mid = mid;
      }
    }
  }

  cout << "Basis for domain:\n";
  for (int i = 0; i < dimDom; i++) {
    cout << i << "\t";
    asciiState(dom[i].sid);
    cout << endl;
  }

  cout << "Basis for codomain:\n";
  for (int i = 0; i < dimCodom; i++) {
    cout << i << "\t";
    printMonom(codom[i].deg, codom[i].mid);
    cout << "\t";
    asciiState(codom[i].sid);
    cout << endl;
  }

  int curState[N];
  int result[N * (N - 1)][N]; // placeholder for diff of that state
  for (int sid = 0; sid < factorial(N); sid++) {
    if (aGrading[sid] == aHom) {
      if (mGrading[sid] == mHom) {
        cout << "d";
        asciiState(sid);
        cout << " = ";
        diff(sid, result);
        for (int tid = 0; tid < choose2(N); tid++) {
          int curSid = transpo2state(sid, tid);
          if (result[tid][0] != -1) {
            int mid = monom2mid(result[tid]);
            int deg = getDeg(result[tid]);
            d[bookmarks[deg] + gid[curSid] * numMonomsDeg(deg) + mid][gid[sid]] = true;
            cout << " + ";
            for (int l = 0; l < N; l++)
              if (result[tid][l] == 1)
                cout << "V" << (l + 1);
            asciiState(curSid);
          }
          int tid2 = tid + N * (N - 1) / 2;
          if (result[tid2][0] != -1) {
            int mid = monom2mid(result[tid2]);
            int deg = getDeg(result[tid2]);
            d[bookmarks[deg] + gid[curSid] * numMonomsDeg(deg) + mid][gid[sid]] = true;
            cout << " + ";
            for (int l = 0; l < N; l++)
              if (result[tid2][l] == 1)
                cout << "V" << (l + 1);
            asciiState(curSid);
          }
        }
        cout << endl;
      } else if (mGrading[sid] == mHom - 1) {

      }
    }
  }

  /*
  // print boolean matrix
  for (int i = 0; i < dimCodom; i++) {
    for (int j = 0; j < dimDom; j++)
      cout << d[i][j] << " ";
    cout << endl;
  }
  */

  initC();
  for (int i = 0; i <= cMax_ / 2; i++) {
    for (int j = 0; j <= cMax_ / 2; j++)
      cout << c_[i][j] << "\t";
    cout << endl;
  }

  cout << chooseBig(67,33);

  return 0;
}

void initOxPoints() {
  int oPerm[N];
  int xPerm[N];
  int i;

  // initialize oPerm and xPerm from ox string (at the top)
  for (i = 0; i < N; i++) {
    oPerm[i] = ox[i] - '0';
    xPerm[i] = ox[N + 1 + i] - '0';
  }

  // print O's and X's
  cout << "O: [" << oPerm[0];
  for (i = 1; i < N; i++) cout << "," << oPerm[i];
  cout << "]\nX: [" << xPerm[0];
  for (i = 1; i < N; i++) cout << "," << xPerm[i];
  cout << "]\n";

  // initialize oPoints and xPoints
  for (i = 0; i < N; i++) {
    oPoints[i].x = i * scaleFactor + scaleFactor / 2;
    oPoints[i].y = oPerm[i] * scaleFactor - scaleFactor / 2;
    xPoints[i].x = i * scaleFactor + scaleFactor / 2;
    xPoints[i].y = xPerm[i] * scaleFactor - scaleFactor / 2;
  }
}

void computeGradings() {
  int state[N]; // placeholder for state generated during some iteration
  Point sPoints[N]; // points of that state
  int i, j, sid;

  // initialize every grading to have dimension 0
  for (i = 0; i <= gradingBound * 2; i++)
    for (j = 0; j <= gradingBound * 2; j++)
      dimension[i][j] = 0;

  // run through all states
  for (sid = 0; sid < factorial(N); sid++) {
    sid2state(sid, state);
    state2points(state, sPoints);

    int ss = eye(sPoints, sPoints);
    int so = eye(sPoints, oPoints);
    int os = eye(oPoints, sPoints);
    int oo = eye(oPoints, oPoints);
    int sx = eye(sPoints, xPoints);
    int xs = eye(xPoints, sPoints);
    int xx = eye(xPoints, xPoints);

    int maslovO = ss - so - os + oo;
    int maslovX = ss - sx - xs + xx;
    int alexander = (maslovO - maslovX - N + 1) / 2;

    if (maslovO < mMin) mMin = maslovO;
    if (maslovO > mMax) mMax = maslovO;
    if (alexander < aMin) aMin = alexander;
    if (alexander > aMax) aMax = alexander;

    mGrading[sid] = maslovO;
    aGrading[sid] = alexander;
    gid[sid] = dimension[z2n(maslovO)][z2n(alexander)]; // id of state in its own grading
    dimension[z2n(maslovO)][z2n(alexander)]++;
  }
}

void printGradings() {
  for (int sid = 0; sid < factorial(N); sid++) {
    asciiState(sid);
    cout << ": (" << mGrading[sid] << "," << aGrading[sid] << ")\n";
  }
}

void printDimensions() {
  int m, a;
  cout << "Dimensions of gradings:\n\t";
  for (m = mMin; m <= mMax; m++)
    cout << m << "\t";
  cout << endl;
  for (a = aMax; a >= aMin; a--) {
    cout << a << "\t";
    for (m = mMin; m <= mMax; m++)
      cout << dimension[z2n(m)][z2n(a)] << "\t";
    cout << endl;
  }
}
