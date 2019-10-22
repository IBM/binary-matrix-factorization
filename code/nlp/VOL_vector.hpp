// Copyright (C) 2019, International Business Machines
// Corporation.  All Rights Reserved.

// This program is distributed under the terms of the
// Eclipse Public License - v 2.0

#include <cfloat>
#include <algorithm>
#include <cstdio>
#include <cmath>


#ifndef VOL_vector_hpp
#define VOL_vector_hpp
#ifndef VOL_DEBUG
// When VOL_DEBUG is 1, we check vector indices
//#define VOL_DEBUG 1
#endif
#if defined(VOL_DEBUG) && (VOL_DEBUG != 0)
#define VOL_TEST_INDEX(i, size)            \
{                        \
if ((i) < 0 || (i) >= (size)) {        \
printf("bad VOL_?vector index\n");    \
abort();                    \
}                        \
}
#define VOL_TEST_SIZE(size)            \
{                        \
if (s <= 0) {                \
printf("bad VOL_?vector size\n");        \
abort();                    \
}                        \
}
#else
#define VOL_TEST_INDEX(i, size)
#define VOL_TEST_SIZE(size)
#endif


#include <stdio.h>

class VOL_dvector {
public:
    /** The array holding the vector */
    double* v;
    /** The size of the vector */
    int sz;
    
public:
    /** Construct a vector of size s. The content of the vector is undefined. */
    VOL_dvector(const int s) {
        VOL_TEST_SIZE(s);
        v = new double[sz = s];
    }
    /** Default constructor creates a vector of size 0. */
    VOL_dvector() : v(0), sz(0) {}
    /** Copy constructor makes a replica of x. */
    VOL_dvector(const VOL_dvector& x) : v(0), sz(0) {
        sz = x.sz;
        if (sz > 0) {
            v = new double[sz];
            std::copy(x.v, x.v + sz, v);
        }
    }
    /** The destructor deletes the data array. */
    ~VOL_dvector() { delete[] v; }
    
    /** Return the size of the vector. */
    inline int size() const {return sz;}
    
    /** Return a reference to the <code>i</code>-th entry. */
    inline double& operator[](const int i) {
        VOL_TEST_INDEX(i, sz);
        return v[i];
    }
    
    /** Return the <code>i</code>-th entry. */
    inline double operator[](const int i) const {
        VOL_TEST_INDEX(i, sz);
        return v[i];
    }
    
    /** Delete the content of the vector and replace it with a vector of length
     0. */
    inline void clear() {
        delete[] v;
        v = 0;
        sz = 0;
    }
    /** Convex combination. Replace the current vector <code>v</code> with
     <code>v = (1-gamma) v + gamma w</code>. */
    inline void cc(const double gamma, const VOL_dvector& w) {
        if (sz != w.sz) {
            printf("bad VOL_dvector sizes\n");
            abort();
        }
        double * p_v = v - 1;
        const double * p_w = w.v - 1;
        const double * const p_e = v + sz;
        const double one_gamma = 1.0 - gamma;
        while ( ++p_v != p_e ){
            *p_v = one_gamma * (*p_v) + gamma * (*++p_w);
        }
    }
    
    /** delete the current vector and allocate space for a vector of size
     <code>s</code>. */
    inline void allocate(const int s) {
        VOL_TEST_SIZE(s);
        delete[] v;
        v = new double[sz = s];
    }
    
    /** swaps the vector with <code>w</code>. */
    inline void swap(VOL_dvector& w) {
        std::swap(v, w.v);
        std::swap(sz, w.sz);
    }
    
    /** Copy <code>w</code> into the vector. */
    VOL_dvector& operator=(const VOL_dvector& w);
    /** Replace every entry in the vector with <code>w</code>. */
    VOL_dvector& operator=(const double w);
    void print(){
        for (int i=0; i<sz; ++i) printf("%g ", v[i]);
        printf("\n");
    }
};



#endif /* VOL_vector_hpp */
