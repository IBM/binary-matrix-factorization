// Copyright (C) 2019, International Business Machines
// Corporation.  All Rights Reserved.

// This program is distributed under the terms of the
// Eclipse Public License - v 2.0

#include <iostream>
#include <algorithm>
#include <string>
#include <sys/times.h>

#include "VOL_vector.hpp"
#include "MatrixPerCols.hpp"
#include "VOL_matrix.hpp"
#include "Solve.hpp"

using namespace std;


void readInputData(string fname, int& n, int& m, VOL_matrix& X);
double optimize(VOL_matrix &V, int rank, bool readSol);

int main(int argc, const char * argv[]) {
    
#if 0
    VOL_matrix V(4,4,1.0);
    V(0,3)=0.;
    V(1,3)=0.;
    V(3,0)=0.;
    V(3,1)=0.;
    V.print();
    printf("\n");
#else
    string inputFileName=argv[1];
    int n, m;
    VOL_matrix V;
    readInputData(inputFileName,n, m, V);
    //V.print();
    //printf("\n");
#endif
    

    VOL_matrix Wmin;
    //MatrixPerCols Hmin;
    VOL_matrix Hmin;
    const int rank=atoi(argv[2]);
    printf("rank %d\n", rank);
    //
    const int optMode=atoi(argv[3]);
    // start time measurement
    double t0;
    struct tms timearr; //clock_t tres;
    times(&timearr);
    t0 = timearr.tms_utime;
    double value=1.e31;
    if (optMode==0){
        value=optimize(V, rank, false);
    }
    else if (optMode==1){
        value=optimize(V, rank, false);
        while(1){
            double newValue=optimize(V, rank, true);
            if ( newValue >= value ) break;
            value=newValue;
        }
    }
    else if (optMode==2){
        value=optimize(V, rank, true);
    }
    else if (optMode==3){
        value=optimize(V, rank, true);
        while(1){
            double newValue=optimize(V, rank, true);
            if ( newValue >= value ) break;
            value=newValue;
        }
    }
    
    
    times(&timearr);
    double t = (timearr.tms_utime-t0)/100.;
    printf("Total Time: %f secs\n", t);
    FILE *tfile=fopen("nlp_time.txt","w");
    fprintf(tfile,"%f\n",t);
    fclose(tfile);
    
    return 0;
}
    
    
double optimize(VOL_matrix &V, int rank, bool readSol){
    double minsum=1.e31, minLamb=-1.0;
    int minK=-1;
    VOL_matrix Wmin;
    VOL_matrix Hmin;

    double lambda=2.8;
    double lmbStep=0.2;
    int kseed=0;
    for (int il=0; il<5; ++il) {
        lambda+=lmbStep;
        printf("lambda: %g\n",lambda);
        Vec<double> distances;
        //int klim=10;
        int klim=5;
        if (readSol) klim=1;
        for (int k=0; k<klim; ++k){
            ++kseed;
            Solve solve(V, 300, rank, kseed, lambda, readSol);
            VOL_matrix &W=solve.Wmin;
            VOL_matrix &H=solve.Hmin;
            VOL_matrix Product=W*H;
            const int nrows=Product.getnrows();
            const int ncols=Product.getncols();
            double sum=0.0;
            for (int i=0; i<nrows; ++i)
                for (int j=0; j<ncols; ++j) sum+=pow(V(i,j)-Product(i,j), 2.0);
            distances.push_back(sum);
            //printf("Distance %g\n", sum);
            if (sum < minsum){
                minLamb=lambda;
                minK=k;
                minsum=sum;
                Wmin=W;
                Hmin=H;
                solve.printValues();
                printf("dist %g k %d lambda %g\n",sum,k,lambda);
            }
        }
        //Wmin.print();
        //printf("\n");
        //Hmin.print();
        //printf("\n");
        VOL_matrix ProdMin=Wmin*Hmin;
        //for (int i=0; i<ProdMin.ve.size(); ++i)ProdMin.ve[i]=std::min(1.0, ProdMin.ve[i]);
        //ProdMin.print();
        const int numb=(int)distances.size();
        printf("Values: ");
        for (int i=0; i<numb; ++i) printf("%g ", distances[i]);
        printf("\n");
    }
    FILE *wfile=fopen("Smatrix.txt","w");
    printf("\nDistance %g, lambda: %g, k: %d\n",minsum, minLamb, minK);
    for (int i=0; i<V.getnrows(); ++i){
        for (int j=0; j<rank; ++j) fprintf(wfile,"%g ",Wmin(i,j));
        fprintf(wfile,"\n");
    }
    fclose(wfile);
    FILE *hfile=fopen("Bmatrix.txt","w");
    for (int i=0; i<rank; ++i){
        for (int j=0; j<V.getncols(); ++j) fprintf(hfile,"%g ",Hmin(i,j));
        fprintf(hfile,"\n");
    }
    fclose(hfile);
    FILE *vfile=fopen("nlp_value.txt","w");
    fprintf(vfile,"%g\n",minsum);
    fclose(vfile);
    return minsum;
}
