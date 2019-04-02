#include <iostream>
#include <vector>
#include "src/star.h"

int main(int argc, char const *argv[]){

	std::vector<std::string> dnas;
	std::vector<std::string> alignment;

	dnas.push_back("ATTGCCATT");
	dnas.push_back("ATGGCCATT");
	dnas.push_back("ATCCAATTTT");
	dnas.push_back("ATCTTCTT");
	dnas.push_back("ACTGACC");

	int match    = 1;
	int mismatch = 1;
	int gap      = 2;
	int score;

	TStar *s = new TStar(match, mismatch, gap);
	s->FSetDna(dnas);
	s->FScores();	
	alignment = s->FGetMultipleAlignment();

	/*alignment.push_back("MQPILLLV");
	alignment.push_back("MLR-LL--");
	alignment.push_back("MK-ILLL-");
	alignment.push_back("MPPVLILV");*/

	score = s->FSumPairs(alignment);

	std::cout << "[score]: " << score << "\n";
	print_vector_t(alignment);
	// s->FPrintScores();

	delete s;
	return 0;
}