#include <sys/types.h>
#include <unistd.h>

#include <iostream>
#include <chrono>
#include <utility>
#include "inc/pm.h"
#include "inc/star.h"
#include "../utils/read.h"

using namespace std::chrono;
typedef PMatrixAlignment    PM;

// build: g++ msa-pm.cpp -o msa-pm.out -pthread
int main(int argc, char const *argv[]){
    vector<string> dnas;
    vector<string> alignments;

    string filename;
    if(argc != 2){
        filename = "../data/Final.txt";
    }
    else{
        filename = "../data/final/" + string(argv[1]);
    }

    load_data(filename, dnas);

    int match    = 1;
    int mismatch = -1;
    int gap      = -1;

    int score;

    high_resolution_clock::time_point tinit;
    high_resolution_clock::time_point tend;
    duration<double> time_span;

    TStar<PM> *msa = new TStar<PM>(match, mismatch, gap);
    msa->set_dna(dnas);
    
    tinit = high_resolution_clock::now();        
        alignments = msa->run();
        score = msa->sum_of_pairs(alignments);
    tend = high_resolution_clock::now();
    time_span = duration_cast<duration<double>>(tend - tinit);
    cout << "[score]: " << score << "\n";
    std::cout << "[time ]: " << time_span.count() << " s\n";

    string comm = "pmap -x " + std::to_string(getpid()) + " | tail -n 2";
    system(comm.c_str());

    delete msa;
    return 0;
}