// Copyright (C) 2019, International Business Machines
// Corporation.  All Rights Reserved.

// This program is distributed under the terms of the
// Eclipse Public License - v 2.0

#include <cstdio>
#include <cstdlib>

#include <fstream>
#include <iostream>
#include <cassert>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

double readValue(string fname);


int main(int argc, char* argv[]) {

  assert(argc==4);
  
  string inputFileName=argv[1];
  string k = argv[2];
  int alg = atoi(argv[3]);
  cout<<"inputFileName = "<<inputFileName<<endl;

  double bestValue = 1.0e30;
  double totalTime = 0.0;

  if(alg == 1) {

    cout<<endl<<"-----------------------------------------------"<<endl;
    string runNLP = "./driver_nlp " + inputFileName + " " + k + " 0";
    cout<<"run = "<<runNLP<<endl;
    system(runNLP.c_str());
    string nameValueNLP = "nlp_value.txt";
    double bestValueNLP = readValue(nameValueNLP);
    string nameTimeNLP = "nlp_time.txt";
    double timeNLP = readValue(nameTimeNLP);
    totalTime += timeNLP;
    cout<<"bestValueNLP = "<<bestValueNLP<<", time = "<<timeNLP<<endl;
    string rmValueNLP = "rm nlp_value.txt";
    system(rmValueNLP.c_str());
    string rmTimeNLP = "rm nlp_time.txt";
    system(rmTimeNLP.c_str());
    bestValue = bestValueNLP;

    cout<<endl<<"-----------------------------------------------"<<endl;
    string run98801 = "./driver_heur " + inputFileName + " " + k + " " + "0.9 0.05 0.25 98801";
    cout<<"run = "<<run98801<<endl;
    system(run98801.c_str());
    string nameValue8801 = "heur_value.txt";
    double bestValue8801 = readValue(nameValue8801);
    string nameTime8801 = "heur_time.txt";
    double time8801 = readValue(nameTime8801);
    totalTime += time8801;
    cout<<"bestValue8801 = "<<bestValue8801<<", time = "<<time8801<<endl;
    string rmValue8801 = "rm heur_value.txt";
    system(rmValue8801.c_str());
    string rmTime8801 = "rm heur_time.txt";
    system(rmTime8801.c_str());

    if (bestValue8801 < bestValue)
      bestValue = bestValue8801;

  }
  else if(alg == 2) {
    cout<<endl<<"-----------------------------------------------"<<endl;
    string run18801 = "./driver_heur " + inputFileName + " " + k + " " + "0.1 0.1 0.2 18801";
    cout<<"run = "<<run18801<<endl;
    system(run18801.c_str());
    string nameValue8801 = "heur_value.txt";
    double bestValue8801 = readValue(nameValue8801);
    string nameTime8801 = "heur_time.txt";
    double time8801 = readValue(nameTime8801);
    totalTime += time8801;
    cout<<"bestValue8801 = "<<bestValue8801<<", time = "<<time8801<<endl;
    string rmValue8801 = "rm heur_value.txt";
    system(rmValue8801.c_str());
    string rmTime8801 = "rm heur_time.txt";
    system(rmTime8801.c_str());
    bestValue = bestValue8801;
  }

  cout<<endl<<"-----------------------------------------------"<<endl;
  cout<<"finalValue: "<<bestValue<<endl;
  printf("finalTime: %f secs\n", totalTime);

  return 0;
}

double readValue(string fname)
{
    
    ifstream infile(fname.c_str());
        
    // read solValue
    string s;
    getline( infile, s );
    istringstream ss( s );
    int counter=1;

    double solValue = -1.0;
    while (ss) {
        string s;
        if (!getline( ss, s, ',' )) break;
        if(counter==1)
	    solValue = atof(s.c_str());
        counter++;
    }

    infile.close();

    return solValue;
}

