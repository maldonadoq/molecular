#include <iostream>
#include "inc/msa-pm.h"

// build: g++ msa-pm.cpp -o msa-pm.out
int main(int argc, char const *argv[]){
    string dnaa = "TACACGCCTCTTCGCC";
	string dnab = "CTGTGAGG";

    int match    = 1;
	int mismatch = -1;
	int gap      = -1;

    PMatrixAlignment *pma = new PMatrixAlignment(match, mismatch, gap);

    TItem res = pma->run(dnaa, dnab);

    cout << "score : " << res.score << "\n";
    cout << "align1: " << res.first << "\n";
    cout << "align2: " << res.second << "\n";
    
    delete pma;
    return 0;
}