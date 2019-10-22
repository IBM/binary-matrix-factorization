// Copyright (C) 2019, International Business Machines
// Corporation.  All Rights Reserved.

// This program is distributed under the terms of the
// Eclipse Public License - v 2.0

#include "VOL_matrix.hpp"
VOL_matrix&
VOL_matrix::operator=(const VOL_matrix& w) {
    if (this == &w)
        return *this;
    nrows=w.nrows;
    ncols=w.ncols;
    ve=w.ve;
    begRow=w.begRow;
    return *this;
}
/// Usage v=w; where w is a double. It copies w in every entry of v
VOL_matrix&
VOL_matrix::operator=(const double w) {
    ve=w;
    return *this;
}
double& VOL_matrix::operator()(const unsigned &rowNo, const unsigned & colNo)
{
    MAT_TEST_row(rowNo, nrows);
    MAT_TEST_col(colNo, ncols);
    return this->ve[rowNo*ncols + colNo];
    //return this->ve[begRow[rowNo] + colNo];
    //return *(this->ve.v+begRow[rowNo] + colNo);
}
// matrix multiplication
VOL_matrix
VOL_matrix::operator*(VOL_matrix & B){
    VOL_matrix multip(nrows,B.getncols());
    if(ncols == B.getnrows())
    {
      unsigned i,j;
        double temp = 0.0;
        const int ncolsB=B.getncols();
        double *p_m=multip.ve.v-1;
        for (i = 0; i < nrows; i++)
        {
            for (j = 0; j < ncolsB; j++)
            {
                temp = 0.0;
                double *p_b = B.ve.v + j;
                double *p_v = (*this).ve.v + begRow[i] -1;
		double *p_end =(*this).ve.v + begRow[i] + ncols -1;
		while(p_v != p_end)
		  //for (k = 0; k < ncols; k++)
                {
                    //temp += (*this)(i,k) * B(k,j);
                    //temp += (*++p_v) * B(k,j);
                    temp += (*++p_v) * (*p_b);
                    p_b += ncolsB;
                    //temp += (*++p_v) * B.ve[B.begRow[k]+j];
                }
#if 0
                multip(i,j) = temp;
#else
                (*++p_m) = temp;
#endif
                //printf(" i j %d %d value %g\n", i, j, multip(i,j));
                //multip.ve.print();
            }
            //cout << endl;
        }
        return multip;
    }
    else
    {
        abort();
    }
}
//-----------------
VOL_matrix
VOL_matrix::operator*(MatrixPerCols & B){
    VOL_matrix multip(nrows,B.getncols());
    if(ncols == B.getnrows())
    {
        unsigned i,j;
        double temp = 0.0;
        for (i = 0; i < nrows; i++)
        {
            for (j = 0; j < B.getncols(); j++)
            {
                temp = 0.0;
                //double *p_v = (*this).ve.v + i*ncols -1;
                double *p_v = (*this).ve.v + begRow[i] -1;
                double *p_end = (*this).ve.v + begRow[i] -1 + ncols;
                double *p_b = B.ve.v + B.begCol[j] -1;
                //for (k = 0; k < ncols; k++)
                while( p_v != p_end )
                {
                    //temp += (*this)(i,k) * B(k,j);
                    //temp += (*++p_v) * B(k,j);
                    temp += (*++p_v) * (*++p_b);
                    //temp += (*++p_v) * B.ve[B.begRow[k]+j];
                }
                multip(i,j) = temp;
                //printf(" i j %d %d value %g\n", i, j, multip(i,j));
                //multip.ve.print();
            }
            //cout << endl;
        }
        return multip;
    }
    else
    {
        abort();
    }
}
