// Copyright (C) 2019, International Business Machines
// Corporation.  All Rights Reserved.

// This program is distributed under the terms of the
// Eclipse Public License - v 2.0

#include "Alg4.hpp"
#include <math.h>
Alg4::Alg4(Vec<Vec<int> > &C, int k, Vec<Vec<int> > &B)
{
    constructS(C, k, B);
}
void Alg4::constructS(Vec<Vec<int> > &C, int k, Vec<Vec<int> > &B)
{
    const int n=(int)C.size();
    const int m=(int)C[0].size();
    //Stranp.resize(0);
    Stranp.clear();
    Stranp.resize(k);
    
    // create all possible 0-1 vectors for a row of S and pick the best
    int ntries = (int)pow(2,k)-1;
    for(int i=0; i<n; i++) {
        Vec<int> smin;
        double vmin=1.0e31;
        
        for(int itry=0; itry<=ntries; itry++) {
            Vec<int> s(k,0);
            for(int jcol=0; jcol<k; jcol++) {
                if((itry>>jcol) & 0x1)
                    s[jcol]=1;
                else
                    s[jcol]=0;
                //      cout<<s[jcol]<<", ";
            }
            //    cout<<endl;
            
            // now we can test if this row s is the best we have found so far
            double value=0.0;
            for(int j=0; j<m; j++) {
                int total=0;
                for(int jcol=0; jcol<k; jcol++) {
                    total += s[jcol]*B[jcol][j];
                }
                value+= (pow(C[i][j]-total, 2.0));
            }
            if(value < vmin) {
                smin=s;
                vmin=value;
            }
        }
        
        //      S.push_back(smin);
        for(int jcol=0; jcol<k; jcol++) {
            Stranp[jcol].push_back(smin[jcol]);
        }
    }
}
