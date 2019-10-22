// Copyright (C) 2019, International Business Machines
// Corporation.  All Rights Reserved.

// This program is distributed under the terms of the
// Eclipse Public License - v 2.0

#include "VOL_ivector.hpp"


/// Usage: v=w; where w is a VOL_ivector
VOL_ivector&
VOL_ivector::operator=(const VOL_ivector& w) {
    if (this == &w)
        return *this;
    delete[] v;
    const int wsz = w.size();
    if (wsz == 0) {
        v = 0;
        sz = 0;
    } else {
        v = new int[sz = wsz];
        for (int i = sz - 1; i >= 0; --i)
            v[i] = w[i];
    }
    return *this;
}
/// Usage v=w; where w is a double. It copies w in every entry of v
VOL_ivector&
VOL_ivector::operator=(const int w) {
    for (int i = sz - 1; i >= 0; --i)
        v[i] = w;
    return *this;
}
