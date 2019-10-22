// Copyright (C) 2019, International Business Machines
// Corporation.  All Rights Reserved.

// This program is distributed under the terms of the
// Eclipse Public License - v 2.0

#ifndef Alg4_hpp
#define Alg4_hpp

#include <stdio.h>
#include "Vec.hpp"
class Alg4{
public:
    Alg4(Vec<Vec<int> > &C, int k, Vec<Vec<int> > &B);
    ~Alg4() {}
    void constructS(Vec<Vec<int> > &C, int k, Vec<Vec<int> > &B);
    Vec<Vec<int> > Stranp;
};
#endif /* Alg4_hpp */
