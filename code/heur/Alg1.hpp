// Copyright (C) 2019, International Business Machines
// Corporation.  All Rights Reserved.

// This program is distributed under the terms of the
// Eclipse Public License - v 2.0

class Alg1{
public:
    Alg1(Vec<Vec<int> > &C, int k, Vec<Vec<int> > &A);
    ~Alg1() {}
    void find_min(Vec<Vec<int> > &C, Vec<Vec<int> > &A, int &imin, Vec<int> &smin);
    Vec<Vec<int> > product(int rowsS, int colsB);
    double cover(Vec<Vec<int> > &C, Vec<Vec<int> > &SB, const int &is,
                       int s, Vec<Vec<int> > &A, int &k);
    double evaluate(Vec<Vec<int> > &C, Vec<Vec<int> > &SB, Vec<Vec<int> > &A,
                    const int & k, Vec<int> &s);
    Vec<Vec<int> > B;
    Vec<Vec<int> > Stranp;
};
