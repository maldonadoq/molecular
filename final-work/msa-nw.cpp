#include <sys/types.h>
#include <unistd.h>

#include <iostream>
#include <utility>
#include "inc/nw.h"
#include "inc/star.h"
#include "../utils/read.h"

typedef TNeedlemanWunsch	NW;

// build: g++ msa-nw.cpp -o msa-nw.out -pthread
int main(int argc, char const *argv[]){
	vector<string> dnas;
	vector<string> alignments;

	string filename = "../data/Final.txt";
	load_data(filename, dnas);

 	int match    = 1;
	int mismatch = -1;
	int gap      = -1;

 	TStar<NW> *msa = new TStar<NW>(match, mismatch, gap);
 	msa->set_dna(dnas);
 	alignments = msa->run();

 	string comm = "pmap -x " + std::to_string(getpid()) + " | tail -n 1";
 	system(comm.c_str());

 	delete msa;
	return 0;
}