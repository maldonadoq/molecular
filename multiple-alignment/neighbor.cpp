#include <iostream>
#include <vector>
#include <chrono>
#include <stdio.h>
#include "src/neighbor.h"

using namespace std::chrono;

int main(int argc, char const *argv[]){

	std::vector<std::string> dnas;
	std::vector<std::string> alignment;

	std::pair<int, int> pmin;

	dnas.push_back("ATTGCCATT");
	dnas.push_back("ATGGCCATT");
	dnas.push_back("ATCCAATTTT");
	dnas.push_back("ATCTTCTT");
	dnas.push_back("ACTGACC");

	int match    = 1;
	int mismatch = 1;
	int gap      = 2;

	TNeighbor *p = new TNeighbor(match, mismatch, gap);

	p->FSetDna(dnas);
	p->FAligments();
	p->FDistances();	
	pmin = p->FQ();

	p->FPrintDistances();
	std::cout << "\n";
	p->FPrintQ();

	std::cout << "\nMinimun: [" << pmin.first << "," << pmin.second << "]\n";

	delete p;
	return 0;
}