// Copyright (C) 2019, International Business Machines
// Corporation.  All Rights Reserved.

// This program is distributed under the terms of the
// Eclipse Public License - v 2.0

#ifndef Alg5p_hpp
#define Alg5p_hpp

#include <stdio.h>
#include "Vec.hpp"
class Alg5p{
public:
    Alg5p(Vec<Vec<int> > &C, int k, Vec<Vec<int> > &S, Vec<Vec<int> > &B);
    ~Alg5p() {}
    double overall_error(Vec<Vec<int> > &C, Vec<Vec<int> > &Stranp, Vec<Vec<int> > &B);
    void find_min_alg5p(Vec<Vec<int> > &B, Vec<Vec<int> > &Stranp,
                       Vec<Vec<int> > &C, int l, Vec<int> &smin);
    Vec<Vec<int> > product_alg5(Vec<Vec<int> > &Stranp, Vec<Vec<int> > &B, int colS);
    double cover_alg5p(Vec<Vec<int> > &C, Vec<Vec<int> > &SB, const int &is,
                      int s, Vec<Vec<int> > &B, int l);
};
#endif /* Alg5p_hpp */
