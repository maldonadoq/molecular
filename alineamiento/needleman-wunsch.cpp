#include <iostream>
#include <utility>
#include "src/needleman-wunsch.h"

unsigned TMAX = 1000;

int main(int argc, char const *argv[]){

	std::string dnaa = "AAAC";
	std::string dnab = "AGC";

	// std::string dnaa = "ACTGATTCA";
	// std::string dnab = "ACGCATCA";

	int match    = 1;
	int mismatch = 1;
	int gap      = 2;
	int score;

	TNeedlemanWunsch *nw = new TNeedlemanWunsch(dnaa, dnab, match, mismatch, gap);
	
	score = nw->FMakeMatrix();
	std::pair<std::string, std::string> alignment = nw->FGetOptimal();

	std::cout << "score: " << score << "\n";
	std::cout << alignment.first << "\n" << alignment.second << "\n";

	delete nw;
	return 0;
}