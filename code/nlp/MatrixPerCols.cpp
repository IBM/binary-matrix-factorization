// Copyright (C) 2019, International Business Machines
// Corporation.  All Rights Reserved.

// This program is distributed under the terms of the
// Eclipse Public License - v 2.0

#include "MatrixPerCols.hpp"

MatrixPerCols&
MatrixPerCols::operator=(const MatrixPerCols& w) {
    if (this == &w)
        return *this;
    nrows=w.nrows;
    ncols=w.ncols;
    ve=w.ve;
    begCol=w.begCol;
    return *this;
}
/// Usage v=w; where w is a double. It copies w in every entry of v
MatrixPerCols&
MatrixPerCols::operator=(const double w) {
    ve=w;
    return *this;
}
double& MatrixPerCols::operator()(const unsigned &rowNo, const unsigned &colNo)
{
    MATCOLS_TEST_row(rowNo, nrows);
    MATCOLS_TEST_col(colNo, ncols);
    return this->ve[colNo*nrows + rowNo];
    //return this->ve[begCol[colNo] + rowNo];
    //return *(this->ve.v+begCol[colNo] + rowNo);
}
