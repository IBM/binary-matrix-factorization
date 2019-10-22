// Copyright (C) 2019, International Business Machines
// Corporation.  All Rights Reserved.

// This program is distributed under the terms of the
// Eclipse Public License - v 2.0

#ifndef Alg6_hpp
#define Alg6_hpp

#include <stdio.h>
#include "Vec.hpp"
class Alg6{
public:
    Alg6(Vec<Vec<int> > &C, int k, Vec<Vec<int> > &A);
    ~Alg6() {}
    void find_minCombination(Vec<Vec<int> > &A, Vec<Vec<int> > &C, Vec<int> &bmin, Vec<int> &smin);
    Vec<Vec<int> > product(int rowsS, int colsB);
    void iterate_Alg1(Vec<Vec<int> > &A, Vec<Vec<int> > &C,
                      Vec<Vec<int> > &SB, Vec<int> &b, Vec<int> &s);
    double findColS(Vec<Vec<int> > &C, Vec<Vec<int> > &SB,
                          Vec<int> &b, Vec<int> &s);
    double findRowB(Vec<Vec<int> > &C, Vec<Vec<int> > &SB,
                          Vec<int> &b, Vec<int> &s);
    double CoverS(Vec<Vec<int> > &C, Vec<Vec<int> > &SB, const int &is,
                  int s, Vec<int> &b);
    double CoverB(Vec<Vec<int> > &C, Vec<Vec<int> > &SB, const int &ib,
                        int b, Vec<int> &s);
    double newEvaluate(Vec<Vec<int> > &C, Vec<Vec<int> > &SB, Vec<int> &s, Vec<int> &b);
    Vec<Vec<int> > B;
    Vec<Vec<int> > Stranp;
};
#endif /* Alg6_hpp */
