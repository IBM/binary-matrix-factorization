// Copyright (C) 2019, International Business Machines
// Corporation.  All Rights Reserved.

// This program is distributed under the terms of the
// Eclipse Public License - v 2.0

#include "BuildMatA.hpp"
BuildMatA::BuildMatA(Vec<Vec<int> > &C, int k, double tresh)
{
    const int m=(int)C[0].size();
    for (int i=0; i<m; ++i){
        A.push_back(One(i, C, tresh, m));
    }
}
Vec<int> BuildMatA::One(const int &i, Vec<Vec<int> > &C, double &tresh, const int &m){
    Vec<int> row;
    for (int j=0; j<m; ++j){
        if ( c(i,j,C) >= tresh ) row.push_back(1);
        else row.push_back(0);
    }
    return row;
}
double BuildMatA::c(const int &i, const int &j, const Vec<Vec<int> > &C){
    const int nrows = (int)C.size();
    double norm=0.0;
    for ( int k=0; k<nrows; ++k) norm+=C[k][i];
    double innerProd=0.0;
    for ( int k=0; k<nrows; ++k) innerProd+=C[k][i]*C[k][j];
    return innerProd/norm;
}
