// Copyright (C) 2019, International Business Machines
// Corporation.  All Rights Reserved.

// This program is distributed under the terms of the
// Eclipse Public License - v 2.0

#ifndef Alg7_hpp
#define Alg7_hpp

#include <stdio.h>
#include "Vec.hpp"
class Alg7{
public:
    Alg7(Vec<Vec<int> > &C, int k, Vec<Vec<int> > &B);
    ~Alg7() {}
    Vec<Vec<int> > Stranp;
};
#endif /* Alg7_hpp */
