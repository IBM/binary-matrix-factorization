// Copyright (C) 2019, International Business Machines
// Corporation.  All Rights Reserved.

// This program is distributed under the terms of the
// Eclipse Public License - v 2.0

#include "Alg8p.hpp"
#include "Alg5p.hpp"
double evaluateSolution(Vec<Vec<int> > &x, Vec<Vec<int> > &B, Vec<Vec<int> > &Stranp);
Alg8p::Alg8p(Vec<Vec<int> > &C, int k, Vec<Vec<int> > &Stranp, Vec<Vec<int> > &B)
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
    int second=evaluateSolution(C, B, Stranp);
    //printf(" Alg8p %d\n", second);
    while(first != second){
        first=second;
        Alg5p alg5p(C, k, Stranp, B);
        Alg5p alg5ptranp(Ctranp, k, B, Stranp);
        second=evaluateSolution(C, B, Stranp);
        //printf(" Alg8p %d\n", second);
    }
}
