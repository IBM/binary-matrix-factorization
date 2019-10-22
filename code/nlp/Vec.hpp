// Copyright (C) 2019, International Business Machines
// Corporation.  All Rights Reserved.

// This program is distributed under the terms of the
// Eclipse Public License - v 2.0

#ifndef __VEC_H__
#define __VEC_H__

#include <vector>
#include <iostream>

#define NO_VEC_BOUNDS_CHECKING 1
// the template class vector version of operator[] does not do a bounds check;
// the "at" operator does, but [] looks better.
// It would be nice to have a version of vector where we could
// turn bounds checking on and off for debugging.
// This is an attempt to do that.
// The idea is to define the simplest derived class we can that
// behaves just like vector, but where operator[] does a bounds check.
// This is a first attempt, there are surely things that have been
// left out (such as custom allocators, etc).
// Add them in as needed. - vra

// Stroustrup recommends making a Vec-style class derive
// from vector as private - p. 405.
// However, that would require us to write wrappers for
// all vector methods.
// I haven't done that.
// We still have the problem that we can do this:
//     Vec<int> foo;
//     vector<int> &bar = foo;
//
// That means we may still end up with unchecked accesses.
// For the time being, we'll just assume this doesn't happen;
// we can always search for "vector" in the code.

// check by default
template<class T>
class Vec : public std::vector<T> {
public:
  typedef typename std::vector<T>::iterator iterator;
  typedef typename std::vector<T>::const_iterator const_iterator;

  explicit Vec() : std::vector<T>() {}
  explicit Vec(size_t sz, T init = T()) : std::vector<T>(sz, init) {}
  Vec(const Vec &x) : std::vector<T>(x) {}

  Vec(iterator first, iterator last) : std::vector<T>(first, last) {}

  // The next two initializers cause a problem on the Sun.
  // I supposedly shouldn't matter what const_iterator is defined to be,
  // but suppose it is just "const T *".
  // Then these two constructors take the same argument types.
  // In principle, the compiler should be able to determine that
  // they do the same thing, but the Sun complains about it.
  // I don't know why I did this version anyway, or why
  // I ended up adding the second.
  //Vec(const_iterator first, const_iterator last) : std::vector<T>(first, last) {}

  Vec(const T *first, const T *last) : std::vector<T>(first, last) {}

  ~Vec() {}

    //#if	(NO_VEC_BOUNDS_CHECKING == 0) FB hack
// override operator[] to get bounds checking...

  // this is the whole point
  T &operator[](size_t i);
  const T  &operator[](size_t i) const;
    //#endif

  // no need for other operator defs, I think

  // use this to avoid code bloat in inlined operator[] calls.
  T &range_error(size_t i);
  const T  &range_error(size_t i) const;

  // be careful...
  T       *beginptr()       { return &std::vector<T>::operator[](0); }
  const T *beginptr() const { return &std::vector<T>::operator[](0); }
  T       *endptr()         { return &std::vector<T>::operator[](this->size()); }
  const T *endptr()   const { return &std::vector<T>::operator[](this->size()); }
};

#if	(NO_VEC_BOUNDS_CHECKING == 0)
template<class T>
inline T &Vec<T>::operator[](size_t i)
{
  if (0<=i && i<this->size())
    return std::vector<T>::operator[](i);

  return range_error(i);
}

template<class T>
inline const T &Vec<T>::operator[](size_t i) const
{
  if (0<=i && i<this->size())
    return std::vector<T>::operator[](i);

  return range_error(i);
}
#else
// FB hack
template<class T>
inline T &Vec<T>::operator[](size_t i)
{
    return std::vector<T>::operator[](i);
}

template<class T>
inline const T &Vec<T>::operator[](size_t i) const
{
    return std::vector<T>::operator[](i);
}
#endif

// we don't want this to be inlined;
// I suspect that if it is in the header file, it will be.
// nuts - if I put this in a source file (say Vec.cpp),
// I get link errors (on AIX) like this:
// ld: 0711-317 ERROR: Undefined symbol: .Vec<NS_Leg*>::range_error(unsigned long)
// I thought it might have something to do with not having type T
// in the function, so I tried returning T.
// that didn't help.
// apparently it has to be inlined.
//
// Hmmm.. I wonder if that was a template instantiation problem...
#if 0
template<class T>
T &Vec<T>::range_error(size_t i)
{
  std::cerr << "Vec deref error:  index==" << i
	    << " size()==" <<  this->size() << std::endl;
  abort();

  return operator[](0);	// just to avoid the compiler warning
}

template<class T>
inline T Vec<T>::range_error(size_t i) const
{
  std::cerr << "Vec deref error:  index==" << i
	    << " size()==" <<  this->size() << std::endl;
  abort();

  return operator[](0);	// just to avoid the compiler warning
}
#else

void Vec_range_error(size_t i, size_t sz);

template<class T>
T &Vec<T>::range_error(size_t i)
{
  Vec_range_error(i, this->size());
  return operator[](i);	// just to avoid the compiler warning
}

template<class T>
inline const T &Vec<T>::range_error(size_t i) const
{
  Vec_range_error(i, this->size());
  return operator[](i);	// just to avoid the compiler warning
}

#endif



// the purify tool may have problems with the vector<bool>
// use this instead
#if 1
typedef Vec<unsigned char> boolVec;
#else
typedef Vec<bool> boolVec;
#endif


#define	FOREACH(i,vec)	for (size_t i=0; i<vec.size(); ++i) 


// It seems to me that templates don't get instantiated unless
// their definition is in the header file.

template<class T>
class IOVecX {
public:
  IOVecX(const Vec<T> &v, char sep)
    : v_(v), sep_(sep) {}

  const Vec<T> &v_;
  char sep_;
};

template<class T>
IOVecX<T> IOVec(const Vec<T> &v, char sep)
{
  return IOVecX<T>(v,sep);
}

template<class T>
std::ostream& operator<<(std::ostream& os, const IOVecX<T> &iox)
{
  const Vec<T> &x = iox.v_;
  char sep = iox.sep_;

   if (x.size())
      os << x[0];
   for (size_t i=1; i<x.size(); ++i)
      os << sep << x[i];
   return (os);
}

template<class T>
std::ostream& operator<<(std::ostream& os, const Vec<T> &x)
{
  os << IOVecX<T>(x, ' ');
  return (os);
}




// If T is a pointer type, this just compares the pointers.
template<class T>
int operator==(const Vec<T> &xs, const Vec<T> &ys)
{
   if (xs.size() == ys.size()) {
      int n=xs.size();
      for (int i=0; i<n; ++i)
	if (! (xs[i] == ys[i]))
	    return (0);
      return (1);
   }

   return (0);
}

template<class T>
int operator!=(const Vec<T> &xs, const Vec<T> &ys)
{
  return !(xs==ys);
}

// these are inefficient, but handy for quick prototyping
template<class T>
Vec<T> operator/(const Vec<T> &xs, const Vec<T> &ys)
{
  assert(xs.size() == ys.size());

  int n=xs.size();
  Vec<T> value(n);

  for (int i=0; i<n; ++i)
    // ???? this is just for prototyping anyway
    if (ys[i])
      value[i] = (xs[i] / ys[i]);
    else
      value[i] = 0.0;

   return (value);
}

inline static 
Vec<double> Double(const Vec<int> &xs)
{
  size_t n= xs.size();
  Vec<double> value(n);

  for (unsigned int i=0; i<n; ++i)
    value[i] = xs[i];

   return (value);
}


#include <algorithm>
template<class T>
bool find(const Vec<T> &vec, const T val)
{
  return (find(vec.beginptr(), vec.endptr(), val) != vec.endptr());
}

template<class T>
bool find(const Vec<T*> &vec, const T* val)
{
  return (find(vec.beginptr(), vec.endptr(), val) != vec.endptr());
}


#endif

