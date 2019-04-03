#include <iostream>
#include <utility>
#include <chrono>
#include <stdio.h>
#include "src/smith-waterman.h"

using namespace std::chrono;

int main(int argc, char const *argv[]){
	std::string dnaa = "ATCGAAAGTACATCGAAAGTACAAATCGAAAGTTACGTATATCGAAAGTACATCGAAAGTACAAATCGAAAGTTACGTATCGAAAGTACCGAAAGTAC";
	std::string dnab = "CGGTCAAGAAGTTACAATGTAAGTACGCTAGCGGTCATTACAGCAGTACCGGTAGAGGAGCAAGAAGTTACAGGGAAA";

	// std::string dnaa = "GTACATCCAGAAGTAGTACGCGTATAGTACGTAACACAACAGTTGGAGGTGTGTCGAGTCGAAGTAACAGGTCACAACTGAAGTACGCGATTAGTACGTACCTGA";
	// std::string dnab = "CAGCATCAGAAGTACGCGTATAGTACGTAACACAACAGTTGGAGGTGTAGTACGCATATAGTACGTACCTGA";

	int match    = 1;
	int mismatch = 1;
	int gap      = 2;
	unsigned n 	 = 10;

	float p = 5;
	float q = 3;

	int score;	

	std::vector<std::pair<std::string, std::string> > alignments;
	std::vector<std::pair<std::string, std::string> > balignments;

	TSmithWaterman *sw = new TSmithWaterman(dnaa, dnab, match, mismatch, gap);

	high_resolution_clock::time_point tinit;
	high_resolution_clock::time_point tend;

	tinit = high_resolution_clock::now();
		score = sw->FMakeMatrix();
	tend = high_resolution_clock::now();
	duration<double> time_span = duration_cast<duration<double>>(tend - tinit);
	std::cout << "[time matrix]: " << time_span.count() << " s\n";
	
	tinit = high_resolution_clock::now();
		alignments = sw->FLocalOptimum(n);
	tend = high_resolution_clock::now();
	time_span = duration_cast<duration<double>>(tend - tinit);
	std::cout << "[time paths] : " << time_span.count() << " s\n";

	tinit = high_resolution_clock::now();
		balignments = sw->FGetBestAlignment(alignments, p, q);
	tend = high_resolution_clock::now();
	time_span = duration_cast<duration<double>>(tend - tinit);
	std::cout << "[time penalty]: " << time_span.count() << " s\n";

	std::cout << "[score]       : " << score << "\n";
	// sw->FPrintWeightMatrix();
	// sw->FPrintBackMatrix();
	std::cout << "[find]        : " << alignments.size() << "\n";

	print_vector_pair_a(alignments);
	std::cout << "-------\n";
	print_vector_pair_a(balignments);

	std::cout << "[n alignments]:\t" << alignments.size() << "\n";
	std::cout << "[b alignments]:\t" << balignments.size() << "\n";

	delete sw;
	return 0;
}