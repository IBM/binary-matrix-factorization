// Copyright (C) 2019, International Business Machines
// Corporation.  All Rights Reserved.

// This program is distributed under the terms of the
// Eclipse Public License - v 2.0

#include <math.h>
#include "Solve.hpp"


double Solve::Sigmoid(double x){
    double y = 1./(1. + exp(-lambda * x));
    return y;
}
double Solve::DerSigm(double x){
    double y = lambda * x * (1. - x);
    return y;
}
double Solve::BoolProd(double x){
    double y= 2.0 * (x - 0.5);
    return y;
}
double Solve::function(VOL_matrix &X){
    const int nrows=X.getnrows();
    const int ncols=X.getncols();
    double sum=0.;
#if 0
    for (int i=0; i<nrows; ++i)
        for (int j=0; j<ncols; ++j)
            sum += pow((X(i,j)-Product(i,j)), 2.);
#else
    const int nm=nrows*ncols;
    double *p_x=X.ve.v-1;
    double *p_p=Product.ve.v-1;
    double *p_end=p_x+nm;
    while (p_x != p_end)
        sum+= pow((*++p_x) - (*++p_p), 2.);
#endif
    //
    sum *= 0.5;
    return sum;
}
void Solve::computeGradient(VOL_matrix &X){
    const int nrows=X.getnrows();
    const int ncols=X.getncols();
    //
    //MatrixPerCols difPCols(nrows,ncols);
    //for (int i=0; i<nrows; ++i)
     //   for (int j=0; j<ncols; ++j)
     //       difPCols(i,j)=X(i,j)-Product(i,j);
    //
    VOL_matrix dif(nrows,ncols);
    const int num = nrows*ncols;
    for (int i=0; i<num; ++i)
        dif.ve[i]=X.ve[i]-Product.ve[i];
    //
    for (int i=0; i<nrows; ++i){
        for (int j=0; j<rank; ++j){
            double sum=0.0;
            double *p_d = dif.ve.v + dif.begRow[i]-1;
            //double *p_end = dif.ve.v + dif.begRow[i] + ncols-1;
            double *p_h = H.ve.v + H.begRow[j]-1;
            for (int l=0; l<ncols; ++l)
            //while ( p_d != p_end )
            {
                //sum-= (X(i,l)-Product(i,l)) * H(j,l);
                //sum-= dif(i,l) * H(j,l);
                //sum-= (*++p_d) * H(j,l);
                sum-= (*++p_d) * (*++p_h);
            }
            gradw(i,j)=sum*DerSigm(W(i,j));
        }
    }
    //
    const int ncolsW=W.getncols();
    for (int i=0; i<rank; ++i){
        for (int j=0; j<ncols; ++j) {
            double sum=0.0;
            //double *p_d = difPCols.ve.v + difPCols.begCol[j]-1;
            double *p_w = W.ve.v + i;
            double *p_d = dif.ve.v + j;
            for (int l=0; l<nrows; ++l){
                //sum-= (X(l,j)-Product(l,j)) * W(l,i);
                //sum-= dif(l,j) * (*++p_w);
                //sum-= (*++p_d) * W(l,i);
                sum-= (*p_d) * (*p_w);
                p_w += ncolsW;
                p_d += ncols;
                //sum-= dif(l,j) * W(l,i);
            }
            gradh(i,j)=sum*DerSigm(H(i,j));
        }
    }
}
void Solve::miniSolver(){
    const int nrows=gradw.getnrows();
    const int ncols=gradh.getncols();
    //
    const int numb=nrows*rank;
    for (int i=0; i<numb; ++i) {
        if (gradw.ve[i] > 0.0) wnext.ve[i]= -box;
        else wnext.ve[i]= box;
    }
    //
    const int numbH=rank*ncols;
    for (int i=0; i<numbH; ++i) {
        if (gradh.ve[i] > 0.0) hnext.ve[i]= -box;
        else hnext.ve[i]= box;
    }
}
void Solve::round(VOL_matrix &W, VOL_matrix &H){
    // round W and H
    const int wsize=W.ve.size();
    const int hsize=H.ve.size();
    double *p_w=W.ve.v -1;
    double *p_h=H.ve.v -1;
    for (int i=0; i<wsize; ++i){
        if ((*++p_w) > 0.5) (*p_w)=1.0;
        else (*p_w)=0.0;
    }
    for (int i=0; i<hsize; ++i){
        if ((*++p_h) > 0.5) (*p_h)=1.0;
        else (*p_h)=0.0;
    }
}
double Solve::intSol(VOL_matrix &X){
    VOL_matrix Wint=W;
    VOL_matrix Hint=H;
    round(Wint, Hint);
    VOL_matrix Product=Wint*Hint;
    const int nrows=Product.getnrows();
    const int ncols=Product.getncols();
    double sum=0.0;
#if 0
    for (int i=0; i<nrows; ++i)
        for (int j=0; j<ncols; ++j) sum+=pow(X(i,j)-Product(i,j), 2.0);
#else
    const int nm=nrows*ncols;
    double *p_x=X.ve.v-1;
    double *p_p=Product.ve.v-1;
    double *p_end=p_x+nm;
    while (p_x != p_end)
        sum+= pow((*++p_x) - (*++p_p), 2.0);
#endif
    if (sum < bestValue){
        Wmin=Wint;
        Hmin=Hint;
        bestValue=sum;
    }
    return sum;
}
void Solve::readSolution(VOL_matrix &X, VOL_matrix &w, VOL_matrix &h){
    FILE *fptr;
    // read S
    if ((fptr = fopen("Smatrix.txt","r")) == NULL){
        printf("Error! opening file Hmatrix.txt");
        // Program exits if the file pointer returns NULL.
        exit(1);
    }
    double value;
    for (int i=0; i<nrows; ++i){
        for (int j=0; j<rank; ++j){
            fscanf(fptr,"%lf", &value);
            //printf("%g ", value);
            W(i,j)=value;
            if( value > 0.5 )
                w(i,j)=box;
            else w(i,j)=-box;
        }
        //printf("\n");
    }
    fclose(fptr);
    // read B -----------------------
    if ((fptr = fopen("Bmatrix.txt","r")) == NULL){
        printf("Error! opening file Wmatrix.txt");
        // Program exits if the file pointer returns NULL.
        exit(1);
    }
    for (int i=0; i<rank; ++i){
        for (int j=0; j<ncols; ++j){
            fscanf(fptr,"%lf", &value);
            H(i,j)=value;
            if( value > 0.5 )
                h(i,j)=box;
            else h(i,j)=-box;
        }
    }
    fclose(fptr);
    //double intVal=intSol(X);
    //printf("int value read %g\n", intVal);
}
//
Solve::Solve(VOL_matrix &X, int max_iter, int rrank, int ktry, double lamb,
             bool readSol){
    //
    lambda=lamb;
    box=10.0;
    bestValue=1.e31;
    //
    long seed=1234 * ktry;
    srand48(seed);
    nrows=X.getnrows();
    ncols=X.getncols();
    rank=rrank;
    // variables
    w.allocate(nrows, rank);
    h.allocate(rank, ncols);
    // sigmoids of vars
    W.allocate(nrows, rank);
    H.allocate(rank, ncols);
    // gradients
    gradw.allocate(nrows, rank);
    gradh.allocate(rank, ncols);
    // vars for minisolver
    wnext.allocate(nrows, rank);
    hnext.allocate(rank, ncols);
    //
    wpull.allocate(nrows, rank);
    hpull.allocate(rank, ncols);
    // Intialize w, W
    const int wsize=w.ve.size();
    if ( readSol ) readSolution(X, w, h);
    else
        for (int i=0; i<wsize; ++i){
            double ran= 2.0 * box * drand48()-box;
            w.ve[i]=ran;
        }
    for (int i=0; i<wsize; ++i)
        W.ve[i]=Sigmoid(w.ve[i]);
    // initialize h, H
    const int hsize=h.ve.size();
    if ( !readSol )
        for (int i=0; i<hsize; ++i){
            double ran= 2.0 * box * drand48()-box;
            h.ve[i]=ran;
        }
    for (int i=0; i<hsize; ++i){
        H.ve[i]=Sigmoid(h.ve[i]);
    }
    //W.print();
    //H.print();
    //
    Product=W*H;
    computeGradient(X);
    miniSolver();
    wpull=wnext;
    hpull=hnext;
    double gamma=0.01;
    const double alpha=0.1;
    // Frank-Wolfe algorithm
    for ( int iter=1; iter < max_iter; ++iter){
      //gamma = 1./(iter+1);  // step size
        //if ( iter%1000 == 0 ) gamma*= 0.5;
        //printf("w\n");
        //w.print();
        //printf("\nh\n");
        //h.print();
        for (int i=0; i<wsize; ++i) W.ve[i]=Sigmoid(w.ve[i]);
        for (int i=0; i<hsize; ++i) H.ve[i]=Sigmoid(h.ve[i]);
        //printf("W\n");
        //W.print();
        //printf("\nH\n");
        //H.print();
        Product=W*H;
        //printf("Product\n");
        //Product.print();
        computeGradient(X);
//        if ( iter%2 == 0){
        if ( iter%10 == 0){
            values.push_back(2*function(X));
            intValues.push_back(intSol(X));
        }
        miniSolver();
        wpull.cc(alpha, wnext);
        hpull.cc(alpha, hnext);
        if ( 0 ){
            w.cc(gamma, wnext);
            h.cc(gamma, hnext);
        }
        else {
            w.cc(gamma, wpull);
            h.cc(gamma, hpull);
        }
    }
    // round W and H
    round(W, H);
}
void Solve::printValues(){
    FILE *fp=fopen("values.txt","w");
    for (int i=0; i<values.size(); ++i) {
        fprintf(fp, "%g\n", values[i]);
    }
    fclose(fp);
    FILE *fi=fopen("intValues.txt","w");
    for (int i=0; i<intValues.size(); ++i) {
        fprintf(fi, "%g\n", intValues[i]);
    }
    fclose(fi);
}

