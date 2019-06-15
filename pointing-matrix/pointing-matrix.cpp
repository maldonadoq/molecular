#include <iostream>
#include "inc/pointing-matrix.h"

using std::cout;

// build: g++ pointing-matrix.cpp -o pointing-matrix.out
int main(int argc, char const *argv[]){

    string dnaa = "GTTGC";
	string dnab = "GTAC";

    int match    = 1;
	int mismatch = -1;
	int gap      = -1;

    PMatrixAlignment *pma = new PMatrixAlignment(dnaa, dnab, match, mismatch, gap);

    pma->run();
    pma->print();

    delete pma;
    return 0;
}