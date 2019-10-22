// Copyright (C) 2019, International Business Machines
// Corporation.  All Rights Reserved.

// This program is distributed under the terms of the
// Eclipse Public License - v 2.0

#include "Alg8.hpp"
#include "Alg5.hpp"
double evaluateSolution(Vec<Vec<int> > &x, Vec<Vec<int> > &B, Vec<Vec<int> > &Stranp);
Alg8::Alg8(Vec<Vec<int> > &C, int k, Vec<Vec<int> > &Stranp, Vec<Vec<int> > &B)
{
    // build transpose
    Vec<Vec<int> > Ctranp;
    const int nrows=(int)C.size();
    const int ncols=(int)C[0].size();
    for (int j=0; j<ncols; ++j){
        Vec<int> column(nrows);
        for (int i=0; i<nrows; ++i)
            column[i]=C[i][j];
        Ctranp.push_back(column);
    }
    double first=-1.0;
    double second=-2.0;
    while(first != second){
        Alg5 alg5(C, k, Stranp, B);
        first=evaluateSolution(C, B, Stranp);
        Alg5 alg5tranp(Ctranp, k, B, Stranp);
        second=evaluateSolution(Ctranp, Stranp, B);
    }
}
