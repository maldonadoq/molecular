#include <iostream>
#include <vector>
#include <chrono>
#include <stdio.h>
#include "src/star.h"

using namespace std::chrono;

int main(int argc, char const *argv[]){

	std::vector<std::string> dnas;
	std::vector<std::string> alignment;

	dnas.push_back("ACGGTGAGTGATCACCTAGCACAGGAGTGATCACCTAACGGTGCAGTCACCCGGCGGAGAGCGGTGCAGTCACCTCGGTGCAGCATGCAAGC");
	dnas.push_back("AACGGTGCAGTAGACGAGTGATCACCTACAGGCACCAGTGCACAAGTTCACCAGTTTAACGAAGTGCAGTCACCAGTTGAACA");
	dnas.push_back("AATTCGGTGCAGTCACCACACCTACACCTAACGGACGGAGTGAGTGATCACCTAGCACACCTACACCTAACGGACGG");
	dnas.push_back("ACCTACAAGTCTAAAGTCTAAGTGGAACACCTCTACGAGTGATCACCTAACGCACTAACGGACGGACTCGGCGGACGCGGACGGACTGACT");
	dnas.push_back("AAACCTAACGGTTCGGTGCAGTCACCACTAACGACCTAGCACACCATCACCTATTGGGAAACGG");
	dnas.push_back("TACACGGAGTGAGTGAACGACGATCAATCACCTATCATCACCGGGATAATCACCTGGGAATTCTAT");
	dnas.push_back("CACCTAAGGGTATAACGTCGAACCGAATAACATCATCACATCACCTATCTATGTGTAATCACGCTATGGGGACCTATGCGG");
	dnas.push_back("CTACAGGCACCAGTCACCTAGCACGGAGAGCGGTGCAGAGGAATAATCACCTGG");
	dnas.push_back("ACCTCAACCTCAGGCAACCCAACACGGCAACCCCTCAGGCAACCCAACAC");
	dnas.push_back("AACACACCAACCAACCTCAGGCTCAGGCCGAGGAGAGAGCCTCAGGCCCTCACCCTCAGGCGGC");

	int match    = 1;
	int mismatch = 1;
	int gap      = 2;
	int score;

	TStar *s = new TStar(match, mismatch, gap);
	s->FSetDna(dnas);

	high_resolution_clock::time_point tinit;
	high_resolution_clock::time_point tend;
	duration<double> time_span;

	tinit = high_resolution_clock::now();
		s->FAligments();
	tend = high_resolution_clock::now();
	time_span = duration_cast<duration<double>>(tend - tinit);
	std::cout << "[scores]    : " << time_span.count() << " s\n";	
	
	tinit = high_resolution_clock::now();
		alignment = s->FGetMultipleAlignment();
	tend = high_resolution_clock::now();
	time_span = duration_cast<duration<double>>(tend - tinit);
	std::cout << "[multiple]  : " << time_span.count() << " s\n";

	/*alignment.push_back("MQPILLLV");
	alignment.push_back("MLR-LL--");
	alignment.push_back("MK-ILLL-");
	alignment.push_back("MPPVLILV");*/

	tinit = high_resolution_clock::now();
		score = s->FSumPairs(alignment);
	tend = high_resolution_clock::now();
	time_span = duration_cast<duration<double>>(tend - tinit);
	std::cout << "[pair's sum]: " << time_span.count() << " s\n";	

	std::cout << "[score]     : " << score << "\n\n";
	print_vector_t(alignment);
	// s->FPrintScores();

	delete s;
	return 0;
}