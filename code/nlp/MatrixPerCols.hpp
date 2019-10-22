// Copyright (C) 2019, International Business Machines
// Corporation.  All Rights Reserved.

// This program is distributed under the terms of the
// Eclipse Public License - v 2.0

#ifndef MatrixPerCols_hpp
#define MatrixPerCols_hpp

#include <stdio.h>
#include "VOL_vector.hpp"
#include "VOL_ivector.hpp"

#ifndef MATCOLS_DEBUG
// When MATCOLS_DEBUG is 1, we check sizes
//#define MATCOLS_DEBUG 1
#endif
#if defined(MATCOLS_DEBUG) && (MATCOLS_DEBUG != 0)
#define MATCOLS_TEST_row(i, nrows)            \
{                        \
if ((i) < 0 || (i) >= (nrows)) {        \
printf("bad matrix index\n");    \
abort();                    \
}                        \
}
#define MATCOLS_TEST_col(i, ncols)            \
{                        \
if ((i) < 0 || (i) >= (ncols)) {        \
printf("bad matrix index\n");    \
abort();                    \
}                        \
}
#define MATCOLS_TEST_SIZE(size)            \
{                        \
if (size <= 0) {                \
printf("bad Matrix size\n");        \
abort();                    \
}                        \
}
#else
#define MATCOLS_TEST_SIZE(size)
#define MATCOLS_TEST_row(i, nrows)
#define MATCOLS_TEST_col(i, ncols)
#endif

class MatrixPerCols {
public:
    VOL_dvector ve;
    /** The size of the vector */
    int nrows;
    int ncols;
    VOL_ivector begCol;
public:
    /** Construct a matrix. The content of the matrix is undefined. */
    MatrixPerCols(const int r, const int c) {
        MATCOLS_TEST_SIZE(r);
        MATCOLS_TEST_SIZE(c)
        nrows = r;
        ncols = c;
        ve.allocate(r*c);
        begCol.allocate(ncols);
        for (int i=0; i<ncols; ++i) begCol[i]=i*nrows;
    }
    MatrixPerCols(const int r, const int c, const double w) {
        MATCOLS_TEST_SIZE(r);
        MATCOLS_TEST_SIZE(c)
        nrows = r;
        ncols = c;
        ve.allocate(r*c);
        ve=w;
        begCol.allocate(ncols);
        for (int i=0; i<ncols; ++i) begCol[i]=i*nrows;
    }
    /** Default constructor creates a vector of size 0. */
    MatrixPerCols() : nrows(0), ncols(0) {}
    /** Copy constructor makes a replica of x. */
    MatrixPerCols(const MatrixPerCols& x) : nrows(0), ncols(0) {
        nrows=x.nrows;
        ncols=x.ncols;
        if (nrows*ncols > 0) {
            ve = x.ve;
            begCol = x.begCol;
        }
    }
    /** The destructor deletes the data array. */
    ~MatrixPerCols() {}
    
    /** Return the size. */
    inline int getnrows() const {return nrows;}
    inline int getncols() const {return ncols;}
    
    double& operator()(const unsigned &, const unsigned &);
    
    
    /** Delete the content of the vector and replace it with a vector of length
     0. */
    inline void clear() {
        nrows=0;
        ncols=0;
        ve.clear();
        begCol.clear();
    }
    /** Convex combination. Replace the current vector <code>v</code> with
     <code>v = (1-gamma) v + gamma w</code>. */
    inline void cc(const double gamma, const MatrixPerCols& w) {
        if (nrows != w.nrows || ncols != w.ncols) {
            printf("bad VOL_matrix sizes\n");
            abort();
        }
        ve.cc(gamma, w.ve);
    }
    
    /** delete the current vector and allocate space for a vector of size
     <code>s</code>. */
    inline void allocate(const int nr, const int nc) {
        MATCOLS_TEST_SIZE(nr);
        MATCOLS_TEST_SIZE(nc);
        nrows=nr;
        ncols=nc;
        ve.allocate(nrows*ncols);
        begCol.allocate(ncols);
        for (int i=0; i<ncols; ++i) begCol[i]=i*nrows;
    }
    
    
    /** Copy <code>w</code> into the vector. */
    MatrixPerCols& operator=(const MatrixPerCols& w);
    /** Replace every entry in the vector with <code>w</code>. */
    MatrixPerCols& operator=(const double w);
    
    
    void print(){
        for (int i=0; i<nrows; ++i){
            for (int j=0; j<ncols; ++j){
                printf("%g ", (*this)(i,j));
            }
            printf("\n");
        }
    }
};

#endif /* MatrixPerCols_hpp */
