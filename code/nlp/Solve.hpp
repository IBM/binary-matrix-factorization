// Copyright (C) 2019, International Business Machines
// Corporation.  All Rights Reserved.

// This program is distributed under the terms of the
// Eclipse Public License - v 2.0

#ifndef Solve_hpp
#define Solve_hpp

#include <stdio.h>
#include "VOL_vector.hpp"
#include "VOL_matrix.hpp"
#include "Vec.hpp"

class Solve {
private:
    double lambda;
    double box;
    int nrows;
    int ncols;
    int rank;
public:
    Solve(VOL_matrix &X, int max_iter, int rrank, int ktry, double lamb,
          bool readSol);
    double Sigmoid(double x);
    double DerSigm(double x);
    double BoolProd(double x);
    double function(VOL_matrix &X);
    void computeGradient(VOL_matrix &X);
    void miniSolver();
    void printValues();
    void round(VOL_matrix &W, VOL_matrix &H);
    double intSol(VOL_matrix &X);
    void readSolution(VOL_matrix &X, VOL_matrix &w, VOL_matrix &h);
    //----------------------
    VOL_matrix Product;
    VOL_matrix w;
    VOL_matrix h;
    VOL_matrix W;
    VOL_matrix H;
    VOL_matrix gradw;
    VOL_matrix gradh;
    VOL_matrix wnext;
    VOL_matrix hnext;
    VOL_matrix wpull;
    VOL_matrix hpull;
    VOL_matrix Wmin;
    VOL_matrix Hmin;
    Vec<double> values;
    Vec<double> intValues;
    double bestValue;
};



#endif /* Solve_hpp */
