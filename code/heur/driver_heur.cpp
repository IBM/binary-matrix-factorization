// Copyright (C) 2019, International Business Machines
// Corporation.  All Rights Reserved.

// This program is distributed under the terms of the
// Eclipse Public License - v 2.0

#include "Vec.hpp"
#include "Alg1.hpp"
#include "BuildMatA.hpp"
#include "Alg7.hpp"
#include "Alg8.hpp"
#include "Alg8p.hpp"
#include "Alg6.hpp"
#include "Alg9.hpp"


#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include <cassert>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#ifndef WIN32
#include <sys/times.h>
#endif

using namespace std;


void readInputData(string fname, int& n, int& m, Vec<Vec<int> >& x);
double evaluateSolution(Vec<Vec<int> > &x, Vec<Vec<int> > &B, Vec<Vec<int> > &Stranp);
Vec<Vec<int> > product(Vec<Vec<int> > &Stranp, Vec<Vec<int> > &B, int rowsS, int colsB);
double optimize(Vec<Vec<int> > &X, int &k, double &trsh, int Alg, Vec<Vec<int> > &S,
                Vec<Vec<int> > &B);
void perturbB(Vec<Vec<int> > &B);
void doubleLoop(Vec<Vec<int> > &X, int &k, double &tresholdStep,
                double &perturbStep, double &maxPerturb,
                int Alg);
double tresholdLoop(Vec<Vec<int> > &X, int &k, double &tresholdStep,
                    double &optTresh, int &Alg, Vec<Vec<int> > &S, Vec<Vec<int> > &B);
double improve(Vec<Vec<int> > &X, int &k, int Alg, Vec<Vec<int> > &B,
            Vec<Vec<int> > &Stranp);
int oldOptimize(Vec<Vec<int> > &X, int &k, double &trsh, int Alg);

void writeMat(Vec<Vec<int> > &StranpMin, Vec<Vec<int> > &Bmin, double minValue);


double toPerturb=-1.;

int main(int argc, const char * argv[]) {
    // start time measurement
    double t0;
    struct tms timearr; //clock_t tres;
    //tres = times(&timearr);
    times(&timearr);
    t0 = timearr.tms_utime;
    
    // read input data file name
    assert(argc==7);
    string inputFileName=argv[1];
    int k=atoi(argv[2]);
    double tresholdStep=atof(argv[3]);
    double perturbStep=atof(argv[4]);
    double maxPerturb=atof(argv[5]);
    int Alg=atoi(argv[6]);
    cout<<"inputFileName="<<inputFileName<<", k="<<k<<", tresholStep="<<tresholdStep<<
    ", perturbStep="<<perturbStep<<
    ", maxPerturbation="<<maxPerturb<<
    ", Alg "<< Alg<<endl;
    
    
    int n, m;
    Vec<Vec<int> > X;
    readInputData(inputFileName,n, m, X);
    printf("n= %d, m= %d\n", n, m);
    
    Vec<Vec<int> > A;
    
    
    doubleLoop(X, k, tresholdStep, perturbStep, maxPerturb, Alg);
    
    // end time measurement
    //tres = times(&timearr);
    times(&timearr);
    double t = (timearr.tms_utime-t0)/100.;
    printf("Total Time: %f secs\n", t);
    FILE *tfile=fopen("heur_time.txt","w");
    fprintf(tfile,"%f\n",t);
    fclose(tfile);
    
    return 0;
}
void doubleLoop(Vec<Vec<int> > &X, int &k, double &tresholdStep,
                double &perturbStep, double &maxPerturb,
                int Alg){
    int d5 = Alg %10;
    double optTresh=-1.;
    Vec<Vec<int> > StranpMin;
    Vec<Vec<int> > Bmin;
    if ( d5==1 ){ // loop for perturbation
        toPerturb=0.0;
        double minValue=1.e31;
        double minPert=-1.;
        double minTresh=-1.;
        while (toPerturb < maxPerturb){
            printf("\nPerturbation %g --------------------------\n",toPerturb);
            Vec<Vec<int> > Stry;
            Vec<Vec<int> > Btry;
            double value=tresholdLoop(X, k, tresholdStep, optTresh, Alg, Stry, Btry);
            if ( value < minValue){
                minValue=value;
                StranpMin=Stry;
                Bmin=Btry;
                minPert=toPerturb;
                minTresh=optTresh;
            }
            toPerturb+=perturbStep;
        }
        printf("\nBest value %g treshold %g perturbation %g\n",
               minValue, minTresh, minPert);
        writeMat(StranpMin, Bmin, minValue);
    }
    else { // no perturbation
        double value=tresholdLoop(X, k, tresholdStep, optTresh, Alg, StranpMin, Bmin);
        printf("\nBest value %g treshold %g \n", value, optTresh);
        writeMat(StranpMin, Bmin, value);
    }
}

double tresholdLoop(Vec<Vec<int> > &X, int &k, double &tresholdStep,
                    double &optTresh, int &Alg, Vec<Vec<int> > &Stry,
                    Vec<Vec<int> > &Btry){
    double minValue=1.e31;
    double minTrsh=-1.;
    double trsh=tresholdStep;
    while ( trsh < 1. ){
        printf("\nTreshold %g\n",trsh);
        Vec<Vec<int> > Scand;
        Vec<Vec<int> > Bcand;
        double value=optimize(X, k, trsh, Alg, Scand, Bcand);
        if ( value < minValue){
            minValue=value;
            Stry=Scand;
            Btry=Bcand;
            minTrsh=trsh;
        }
        trsh+=tresholdStep;
    }
    //printf("\nBest value %d treshold %g\n", minValue, minTrsh);
    optTresh=minTrsh;
    return minValue;
}


double optimize(Vec<Vec<int> > &X, int &k, double &trsh, int Alg,
                Vec<Vec<int> > &Scand, Vec<Vec<int> > &Bcand)
{
   int newAlg=Alg;
    // get digits from Alg
    int d5 = Alg %10;
    Alg /= 10;
    int d4 = Alg %10;
    Alg /= 10;
    int d3 = Alg %10;
    Alg /= 10;
    int d2 = Alg %10;
    Alg /= 10;
    int d1 = Alg %10;
    printf("Algorithms %d %d %d %d %d\n", d1, d2, d3, d4, d5);
    BuildMatA buildA(X, k, trsh);
    Vec<Vec<int> > &A=buildA.A;
    Vec<Vec<int> > B;
    Vec<Vec<int> > Stranp;
    if (d1==1){
        Alg1 alg1(X, k, A);
        B=alg1.B;
        Stranp=alg1.Stranp;
    }
    else
        if (d1==9){
            Alg9 alg9(X, k);
            B=alg9.B;
            Stranp=alg9.Stranp;
        }
        else
        {
            Alg6 alg6(X, k, A);
            B=alg6.B;
            Stranp=alg6.Stranp;
        }
    
    double result=evaluateSolution(X, B, Stranp);
    Scand=Stranp;
    Bcand=B;
    // improve solution
    while (1){
        double newResult=improve(X, k, newAlg, B, Stranp);
        if (newResult < result) {
            result=newResult;
            Scand=Stranp;
            Bcand=B;
        }
        else break;
    }
    return result;
}

double evaluateSolution(Vec<Vec<int> > &x, Vec<Vec<int> > &B, Vec<Vec<int> > &Stranp)
{
    const int nrows=(int)x.size();
    const int ncols=(int)x[0].size();
    Vec<Vec<int> > SB=product(Stranp, B, nrows, ncols);
    double count=0.0;
    for (int i=0; i<nrows; ++i)
        for (int j=0; j<ncols; ++j)
            count+=pow(x[i][j]-SB[i][j], 2.);
    printf("discrepancy %g \n", count);
    return count;
}
Vec<Vec<int> > product(Vec<Vec<int> > &Stranp, Vec<Vec<int> > &B, int rowsS, int colsB)
{
    Vec<Vec<int> > SB;
    const int rowsB=(int)B.size();
    if ( rowsB==0 ){
        Vec<int> row(colsB,0);
        for ( int irow=0; irow < rowsS; ++irow)
            SB.push_back(row);
    }
    else {
        Vec<int> row(colsB);
        for (int irow=0; irow<rowsS; ++irow){
            for (int icol=0; icol<colsB; ++icol){
                int sum=0;
                for (int k=0; k<rowsB; ++k)
                    sum+=Stranp[k][irow]*B[k][icol];
                row[icol]= sum;
            }
            SB.push_back(row);
        }
    }
    return SB;
}
void perturbB(Vec<Vec<int> > &B)
{
    const int nrows=(int)B.size();
    const int ncols=(int)B[0].size();
    for (int i=0; i<nrows; ++i){
        for (int j=0; j < ncols; ++j) {
            if (drand48() < 1. - toPerturb)continue;
            B[i][j]= 1 - B[i][j];
        }
    }
}
double improve(Vec<Vec<int> > &X, int &k, int Alg, Vec<Vec<int> > &B,
            Vec<Vec<int> > &Stranp)
{
    // get digits from Alg
    int d5 = Alg %10;
    Alg /= 10;
    int d4 = Alg %10;
    Alg /= 10;
    int d3 = Alg %10;
    Alg /= 10;
    int d2 = Alg %10;
    double result=1.e31;
    if (d2==7){
        Alg7 alg7(X, k, B);
        Stranp=alg7.Stranp;
        printf("Alg7 ");
        result=evaluateSolution(X, B, Stranp);
        if (d5==1){
            perturbB(B);
            printf("perturbed ");
            result=evaluateSolution(X, B, Stranp);
        }
    }
    if (d2==8){
        Alg8p alg8(X, k, Stranp, B);
        printf("Alg8 ");
        result=evaluateSolution(X, B, Stranp);
        if (d5==1){
            perturbB(B);
            printf("perturbed ");
            result=evaluateSolution(X, B, Stranp);
        }
    }
    if (d3==8){
        Alg8p alg8(X, k, Stranp, B);
        printf("Alg8 ");
        result=evaluateSolution(X, B, Stranp);
    }
    if (d4==7){
        Alg7 alg7(X, k, B);
        Stranp=alg7.Stranp;
        printf("Alg7 ");
        result=evaluateSolution(X, B, Stranp);
    }
    return result;
}
void writeMat(Vec<Vec<int> > &Stranp, Vec<Vec<int> > &B, double minValue ){
    FILE *sfile=fopen("Sout.txt","w");
    const int nrows=Stranp[0].size();
    const int rank=Stranp.size();
    for (int i=0; i<nrows; ++i){
        for (int j=0; j<rank; ++j) fprintf(sfile,"%d ",Stranp[j][i]);
        fprintf(sfile,"\n");
    }
    fclose(sfile);
    FILE *bfile=fopen("Bout.txt","w");
    const int ncols=B[0].size();
    for (int i=0; i<rank; ++i){
        for (int j=0; j<ncols; ++j) fprintf(bfile,"%d ",B[i][j]);
        fprintf(bfile,"\n");
    }
    fclose(bfile);
    FILE *vfile=fopen("heur_value.txt","w");
    fprintf(vfile,"%g\n",minValue);
    fclose(vfile);
}
