// Copyright (C) 2019, International Business Machines
// Corporation.  All Rights Reserved.

// This program is distributed under the terms of the
// Eclipse Public License - v 2.0

#include "Alg6.hpp"
#include <math.h>
Alg6::Alg6(Vec<Vec<int> > &C, int k, Vec<Vec<int> > &A)
{
    // find rows for B and cols for S using A
    for (int l=0; l < k; ++l){
        Vec<int> s;
        Vec<int> b;
        find_minCombination(A, C, b, s);
        B.push_back(b);
        Stranp.push_back(s);
    }
}
void Alg6::find_minCombination(Vec<Vec<int> > &A, Vec<Vec<int> > &C, Vec<int> &bmin, Vec<int> &smin)
{
    const int m=(int)A.size();
    const int rowsS=(int)C.size();
    const int colsB=(int)C[0].size();
    double vmin=1.e31;
    Vec<Vec<int> > SB = product(rowsS, colsB);
    for (int i=0; i<m; ++i){ // rows of A
        Vec<int> b=A[i];
        Vec<int> s(rowsS);
        iterate_Alg1(A, C, SB, b, s);
        double value=newEvaluate(C, SB, s, b);
        //printf("i %d value %g ----------\n",i,value);
        if (value < vmin){
            vmin=value;
            bmin=b;
            smin=s;
        }
    }
}
Vec<Vec<int> > Alg6::product(int rowsS, int colsB)
{
    Vec<Vec<int> > SB;
    const int rowsB=(int)B.size();
    if ( rowsB==0 ){
        Vec<int> row(colsB,0);
        for ( int irow=0; irow < rowsS; ++irow)
            SB.push_back(row);
    }
    else {
        Vec<int> row(colsB);
        for (int irow=0; irow<rowsS; ++irow){
            for (int icol=0; icol<colsB; ++icol){
                int sum=0;
                for (int k=0; k<rowsB; ++k)
                    sum+=Stranp[k][irow]*B[k][icol];
                row[icol]=sum;
            }
            SB.push_back(row);
        }
    }
    return SB;
}
void Alg6::iterate_Alg1(Vec<Vec<int> > &A, Vec<Vec<int> > &C,
                  Vec<Vec<int> > &SB, Vec<int> &b, Vec<int> &s){
    double first=1.e31, second=1.e31;
    //printf("iterate_alg1----\n");
    while (1){
        first=findColS(C, SB, b, s);
        //printf("first %g\n",first);
        //if(first > second ) printf("***imp first second %g %g\n",first, second);
        if ( first == second ) break;
        second=findRowB(C, SB, b, s);
        //printf("second %g\n",second);
        if (second > first) abort();
        if ( first == second ) break;
    }
    //printf("first second %g %g\n",first, second);
}
double Alg6::findColS(Vec<Vec<int> > &C, Vec<Vec<int> > &SB,
                Vec<int> &b, Vec<int> &s){
    const int rowsS=(int)C.size();
    for (int is=0; is<rowsS; ++is){
        int zeroOne=0;
        if ( CoverS(C, SB, is, 1, b) < CoverS(C, SB, is, 0, b) ) zeroOne=1;
        s[is] = zeroOne;
    }
    double value=newEvaluate(C, SB, s, b);
    return value;
}
double Alg6::findRowB(Vec<Vec<int> > &C, Vec<Vec<int> > &SB,
                Vec<int> &b, Vec<int> &s){
    const int colsB=(int)C[0].size();
    for (int ib=0; ib<colsB; ++ib){
        int zeroOne=0;
        if ( CoverB(C, SB, ib, 1, s) < CoverB(C, SB, ib, 0, s) ) zeroOne=1;
        b[ib] = zeroOne;
    }
    double value=newEvaluate(C, SB, s, b);
    return value;
}
double Alg6::CoverS(Vec<Vec<int> > &C, Vec<Vec<int> > &SB, const int &is,
              int s, Vec<int> &b) {
    const int ncols=(int)C[0].size();
    double countp=0.0;
    for ( int j=0; j<ncols; ++j){
        int sob = SB[is][j] + s * b[j];
        countp+= pow(sob - C[is][j], 2.0);
    }
    return countp;
}
double Alg6::CoverB(Vec<Vec<int> > &C, Vec<Vec<int> > &SB, const int &ib,
              int b, Vec<int> &s) {
    const int nrows=(int)C.size();
    double countp=0.0;
    for ( int i=0; i<nrows; ++i){
        int sob = SB[i][ib] + b * s[i];
        countp+= pow(sob - C[i][ib], 2.0);
    }
    return countp;
}
double Alg6::newEvaluate(Vec<Vec<int> > &C, Vec<Vec<int> > &SB, Vec<int> &s, Vec<int> &b){
    const int nrows=(int)C.size();
    const int ncols=(int)C[0].size();
    double countp=0.0;
    for (int i=0; i<nrows; ++i){
        for (int j=0; j<ncols; ++j){
            int sob=SB[i][j]+s[i]*b[j];
            countp+= pow(sob - C[i][j], 2.0);
        }
    }
    return countp;
}
