// Copyright (C) 2019, International Business Machines
// Corporation.  All Rights Reserved.

// This program is distributed under the terms of the
// Eclipse Public License - v 2.0

#ifndef Alg8p_hpp
#define Alg8p_hpp

#include <stdio.h>
#include "Vec.hpp"
class Alg8p{
public:
    Alg8p(Vec<Vec<int> > &C, int k, Vec<Vec<int> > &Stranp, Vec<Vec<int> > &B);
    ~Alg8p() {}
};
#endif /* Alg8p_hpp */
