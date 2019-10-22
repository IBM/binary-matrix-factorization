// Copyright (C) 2019, International Business Machines
// Corporation.  All Rights Reserved.

// This program is distributed under the terms of the
// Eclipse Public License - v 2.0

#include <cfloat>
#include <algorithm>
#include <cstdio>
#include <cmath>


#ifndef VOL_ivector_hpp
#define VOL_ivector_hpp
#ifndef VOL_iDEBUG
// When VOL_iDEBUG is 1, we check vector indices
//#define VOL_iDEBUG 1
#endif
#if defined(VOL_iDEBUG) && (VOL_iDEBUG != 0)
#define VOL_iTEST_INDEX(i, size)            \
{                        \
if ((i) < 0 || (i) >= (size)) {        \
printf("bad VOL_?vector index\n");    \
abort();                    \
}                        \
}
#define VOL_iTEST_SIZE(size)            \
{                        \
if (s <= 0) {                \
printf("bad VOL_?vector size\n");        \
abort();                    \
}                        \
}
#else
#define VOL_iTEST_INDEX(i, size)
#define VOL_iTEST_SIZE(size)
#endif


#include <stdio.h>

class VOL_ivector {
public:
    /** The array holding the vector */
    int* v;
    /** The size of the vector */
    int sz;
    
public:
    /** Construct a vector of size s. The content of the vector is undefined. */
    VOL_ivector(const int s) {
        VOL_iTEST_SIZE(s);
        v = new int[sz = s];
    }
    /** Default constructor creates a vector of size 0. */
    VOL_ivector() : v(0), sz(0) {}
    /** Copy constructor makes a replica of x. */
    VOL_ivector(const VOL_ivector& x) : v(0), sz(0) {
        sz = x.sz;
        if (sz > 0) {
            v = new int[sz];
            std::copy(x.v, x.v + sz, v);
        }
    }
    /** The destructor deletes the data array. */
    ~VOL_ivector() { delete[] v; }
    
    /** Return the size of the vector. */
    inline int size() const {return sz;}
    
    /** Return a reference to the <code>i</code>-th entry. */
    inline int& operator[](const int i) {
        VOL_iTEST_INDEX(i, sz);
        return v[i];
    }
    
    /** Return the <code>i</code>-th entry. */
    inline int operator[](const int i) const {
        VOL_iTEST_INDEX(i, sz);
        return v[i];
    }
    
    /** Delete the content of the vector and replace it with a vector of length
     0. */
    inline void clear() {
        delete[] v;
        v = 0;
        sz = 0;
    }
  
    /** delete the current vector and allocate space for a vector of size
     <code>s</code>. */
    inline void allocate(const int s) {
        VOL_iTEST_SIZE(s);
        delete[] v;
        v = new int[sz = s];
    }
    
    /** swaps the vector with <code>w</code>. */
    inline void swap(VOL_ivector& w) {
        std::swap(v, w.v);
        std::swap(sz, w.sz);
    }
    
    /** Copy <code>w</code> into the vector. */
    VOL_ivector& operator=(const VOL_ivector& w);
    /** Replace every entry in the vector with <code>w</code>. */
    VOL_ivector& operator=(const int w);
    void print(){
        for (int i=0; i<sz; ++i) printf("%d ", v[i]);
        printf("\n");
    }
};



#endif /* VOL_ivector_hpp */
