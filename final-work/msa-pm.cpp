#include <sys/types.h>
#include <unistd.h>

#include <iostream>
#include <utility>
#include "inc/pm.h"
#include "inc/star.h"
#include "../utils/read.h"

typedef PMatrixAlignment    PM;

// build: g++ msa-pm.cpp -o msa-pm.out -pthread
int main(int argc, char const *argv[]){
    vector<string> dnas;
    vector<string> alignments;

    string filename = "../data/Final.txt";
    load_data(filename, dnas);

    int match    = 1;
    int mismatch = -1;
    int gap      = -1;

    TStar<PM> *msa = new TStar<PM>(match, mismatch, gap);
    msa->set_dna(dnas);
    alignments = msa->run();

    string comm = "pmap -x " + std::to_string(getpid()) + " | tail -n 1";
    system(comm.c_str());

    delete msa;
    return 0;
}

// total           639000K