#include <iostream>
#include "inc/pointing-matrix.h"

// build: g++ pointing-matrix.cpp -o pointing-matrix.out
int main(int argc, char const *argv[]){

    // string dnaa = "GTTGC";
	// string dnab = "GTAC";

    // string dnaa = "TCGGTAGAATCCGTCT";
	// string dnab = "CGAACTATTCGGACGC";

    string dnaa = "TACACGCCTCTTCGCC";
	string dnab = "CTGTGAGG";

    int match    = 1;
	int mismatch = -1;
	int gap      = -2;

    PMatrixAlignment *pma = new PMatrixAlignment(dnaa, dnab, match, mismatch, gap);

    pair<string, string> alignm = pma->run();
    print_pair(alignm);
    
    delete pma;
    return 0;
}