#include <iostream>
#include <vector>
#include "src/star.h"

int main(int argc, char const *argv[]){

	std::vector<std::string> dnas;

	dnas.push_back("ATTGCCATT");
	dnas.push_back("ATGGCCATT");
	dnas.push_back("ATCCAATTTT");
	dnas.push_back("ATCTTCTT");
	dnas.push_back("ACTGACC");

	int match    = 1;
	int mismatch = 1;
	int gap      = 2;

	TStar *s = new TStar(match, mismatch, gap);
	// s->FSetDna(dnas);

	delete s;
	
	return 0;
}