// Copyright (C) 2019, International Business Machines
// Corporation.  All Rights Reserved.

// This program is distributed under the terms of the
// Eclipse Public License - v 2.0

#ifndef VOL_matrix_hpp
#define VOL_matrix_hpp

#include <stdio.h>
#include "VOL_vector.hpp"
#include "VOL_ivector.hpp"
#include "MatrixPerCols.hpp"

#ifndef MAT_DEBUG
// When MAT_DEBUG is 1, we check sizes
//#define MAT_DEBUG 1
#endif
#if defined(MAT_DEBUG) && (MAT_DEBUG != 0)
#define MAT_TEST_row(i, nrows)            \
{                        \
if ((i) < 0 || (i) >= (nrows)) {        \
printf("bad VOL_?matrix index\n");    \
abort();                    \
}                        \
}
#define MAT_TEST_col(i, ncols)            \
{                        \
if ((i) < 0 || (i) >= (ncols)) {        \
printf("bad VOL_?matrix index\n");    \
abort();                    \
}                        \
}
#define MAT_TEST_SIZE(size)            \
{                        \
if (size <= 0) {                \
printf("bad Matrix size\n");        \
abort();                    \
}                        \
}
#else
#define MAT_TEST_SIZE(size)
#define MAT_TEST_row(i, nrows)
#define MAT_TEST_col(i, ncols)
#endif

class VOL_matrix {
public:
    VOL_dvector ve;
    /** The size of the vector */
    int nrows;
    int ncols;
    VOL_ivector begRow;
public:
    /** Construct a matrix. The content of the matrix is undefined. */
    VOL_matrix(const int r, const int c) {
        MAT_TEST_SIZE(r);
        MAT_TEST_SIZE(c)
        nrows = r;
        ncols = c;
        ve.allocate(r*c);
        begRow.allocate(nrows);
        for (int i=0; i<nrows; ++i) begRow[i]=i*ncols;
    }
    VOL_matrix(const int r, const int c, const double w) {
        MAT_TEST_SIZE(r);
        MAT_TEST_SIZE(c)
        nrows = r;
        ncols = c;
        ve.allocate(r*c);
        ve=w;
        begRow.allocate(nrows);
        for (int i=0; i<nrows; ++i) begRow[i]=i*ncols;
    }
    /** Default constructor creates a vector of size 0. */
    VOL_matrix() : nrows(0), ncols(0) {}
    /** Copy constructor makes a replica of x. */
    VOL_matrix(const VOL_matrix& x) : nrows(0), ncols(0) {
        nrows=x.nrows;
        ncols=x.ncols;
        if (nrows*ncols > 0) {
            ve = x.ve;
            begRow = x.begRow;
        }
    }
    /** The destructor deletes the data array. */
    ~VOL_matrix() {}
    
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
        begRow.clear();
    }
    /** Convex combination. Replace the current vector <code>v</code> with
     <code>v = (1-gamma) v + gamma w</code>. */
    inline void cc(const double gamma, const VOL_matrix& w) {
        if (nrows != w.nrows || ncols != w.ncols) {
            printf("bad VOL_matrix sizes\n");
            abort();
        }
        ve.cc(gamma, w.ve);
    }
    
    /** delete the current vector and allocate space for a vector of size
     <code>s</code>. */
    inline void allocate(const int nr, const int nc) {
        MAT_TEST_SIZE(nr);
        MAT_TEST_SIZE(nc);
        nrows=nr;
        ncols=nc;
        ve.allocate(nrows*ncols);
        begRow.allocate(nrows);
        for (int i=0; i<nrows; ++i) begRow[i]=i*ncols;
    }
    
    
    /** Copy <code>w</code> into the vector. */
    VOL_matrix& operator=(const VOL_matrix& w);
    /** Replace every entry in the vector with <code>w</code>. */
    VOL_matrix& operator=(const double w);
    
    VOL_matrix operator*(VOL_matrix &);
    VOL_matrix operator*(MatrixPerCols &);
    
    void print(){
        for (int i=0; i<nrows; ++i){
            for (int j=0; j<ncols; ++j){
                    printf("%g ", (*this)(i,j));
                }
            printf("\n");
        }
    }
};
#endif /* VOL_matrix_hpp */
