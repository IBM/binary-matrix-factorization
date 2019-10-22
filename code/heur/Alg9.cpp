// Copyright (C) 2019, International Business Machines
// Corporation.  All Rights Reserved.

// This program is distributed under the terms of the
// Eclipse Public License - v 2.0

#include "Vec.hpp"
#include "Alg9.hpp"
#include <stdio.h>

Alg9::Alg9(Vec<Vec<int> > &C, int k){
    const int nrows=(int)C.size();
    const int ncols=(int)C[0].size();
    FILE *fptr;
    
    if ((fptr = fopen("Bmatrix.txt","r")) == NULL){
        printf("Error! opening file Bmatrix.txt");
        // Program exits if the file pointer returns NULL.
        exit(1);
    }
    double value;
    for (int i=0; i<k; ++i){
        Vec<int> row;
        for (int j=0; j<ncols; ++j){
            fscanf(fptr,"%lf", &value);
            row.push_back(value);
            //printf("%g ", value);
        }
        //printf("\n");
        //for (int j=0; j<k; ++j) printf("%d ", row[j]);
        //printf("\n");
        B.push_back(row);
    }
    fclose(fptr);
    //-------------------------------------
    if ((fptr = fopen("Smatrix.txt","r")) == NULL){
        printf("Error! opening file Smatrix.txt");
        // Program exits if the file pointer returns NULL.
        exit(1);
    }
    Vec <int> rs(nrows,-1);
    for (int i=0; i<k; ++i) Stranp.push_back(rs);
    for (int i=0; i<nrows; ++i){
        for (int j=0; j<k; ++j){
            fscanf(fptr,"%lf", &value);
            //printf("%g ", value);
            Stranp[j][i]=value;
        }
        //printf("\n");
    }
    fclose(fptr);
}
