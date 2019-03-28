#include <iostream>
#include <utility>
#include<chrono>
#include<stdio.h>
#include "src/needleman-wunsch.h"

using namespace std::chrono;

int main(int argc, char const *argv[]){
	std::string dnaa = "ACGGTGCACAAGTTCACCAGTTGAACAAATTCGGTGCAGTCACCATAACGGTGCAGTCACCAGGCGGTGCAGTCACCCGGCGGTCGGTGCAGCATGCAAGCAACGGTGCAGTCACCAGGCACCAGTGCACAAGTTCACCAGTTTAACGAAGTGCAGTCACC";	
	std::string dnab = "ACCGTGCAGTTGAACATCGGTGCAGAATTCGGTGCAGTCACCATAACGGTCGGTGCAGTGCAGTCACCAGGCGGTGCAGTCACCCGGCGGTCGGTGCAGCATGCAAGCAACGGTGCAGTCACCAGGCACCATCAGTCACCAGGCACCACCAGCGGTGCAG";

	int match    = 1;
	int mismatch = 1;
	int gap      = 2;
	unsigned n 	 = 20;

	int score;
	std::vector<std::pair<std::string, std::string> > alignments;

	TNeedlemanWunsch *nw = new TNeedlemanWunsch(dnaa, dnab, match, mismatch, gap);

	high_resolution_clock::time_point tinit;
	high_resolution_clock::time_point tend;

	tinit = high_resolution_clock::now();
		score = nw->FMakeMatrix();
	tend = high_resolution_clock::now();
	duration<double> time_span = duration_cast<duration<double>>(tend - tinit);
	std::cout << "[time matrix]: " << time_span.count() << " s\n";
	
	tinit = high_resolution_clock::now();
		alignments = nw->FGlobalOptimum(n);
	tend = high_resolution_clock::now();
	time_span = duration_cast<duration<double>>(tend - tinit);
	std::cout << "[time paths] : " << time_span.count() << " s\n";

	std::cout << "[score]      : " << score << "\n";
	// nw->FPrintWeightMatrix();
	// nw->FPrintBackMatrix();
	std::cout << "[find]       : " << alignments.size() << "\n";
	print_vector_pair_a(alignments);

	delete nw;
	return 0;
}
