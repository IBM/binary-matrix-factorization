// Copyright (C) 2019, International Business Machines
// Corporation.  All Rights Reserved.

// This program is distributed under the terms of the
// Eclipse Public License - v 2.0

#ifndef BuildMatA_hpp
#define BuildMatA_hpp

#include <stdio.h>
#include "Vec.hpp"
class BuildMatA{
public:
    BuildMatA(Vec<Vec<int> > &C, int k, double tresh);
    ~BuildMatA() {}
    Vec<int> One(const int &i, Vec<Vec<int> > &C, double &tresh, const int &m);
    double c(const int &i, const int &j, const Vec<Vec<int> > &C);
    Vec<Vec<int> > A;
};

#endif /* BuildMatA_hpp */
