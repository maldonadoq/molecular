#include <iostream>
#include "src/smith-waterman.h"

unsigned TMAX = 1000;

int main(int argc, char const *argv[]){
	
	// std::string dnaa = "ACACACGG";
	// std::string dnab = "ACCCG";

	std::string dnaa = "ACACAAAGGTTTACGG";
	std::string dnab = "ACCCAAATTTAG";

	int match    = 1;
	int mismatch = 1;
	int gap      = 4;

	TSmithWaterman *sw = new TSmithWaterman(dnaa, dnab, match, mismatch, gap);
	sw->FMakeMatrix();
	std::pair<std::string, std::string> alignment = sw->FLocalOptimum();

	//sw->FPrintMatrix();

	std::cout << alignment.first << "\n" << alignment.second << "\n";

	delete sw;

	return 0;
}