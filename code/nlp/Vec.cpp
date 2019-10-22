// Copyright (C) 2019, International Business Machines
// Corporation.  All Rights Reserved.

// This program is distributed under the terms of the
// Eclipse Public License - v 2.0

// ?? this compiles without this include.  does vector declare std::cerr?
#include <iostream>

#include "Vec.hpp"

#if 0

// we don't want this to be inlined;
// I suspect that if it is in the header file, it will be.
template<class T>
T &Vec<T>::range_error(size_t i)
{
  std::cerr << "Vec deref error:  index==" << i
	    << " size()==" <<  size() << std::endl;
  abort();

  return operator[](0);	// just to avoid the compiler warning
}

template<class T>
inline T Vec<T>::range_error(size_t i) const
{
  std::cerr << "Vec deref error:  index==" << i
	    << " size()==" <<  size() << std::endl;
  abort();

  return operator[](0);	// just to avoid the compiler warning
}



#else

// note that this isn't a template function
void Vec_range_error(size_t i, size_t sz)
{
  std::cerr << "Vec deref error:  index==" << i
	    << " size()==" <<  sz << std::endl;
  abort();
}


#endif


