#include <iostream>
#include <utility>
#include <unistd.h>
#include <sys/time.h>
#include <list>
#include "src/needleman-wunsch.h"

int main(int argc, char const *argv[]){

	std::string dnaa = "AAAC";
	std::string dnab = "AGC";

	int match    = 1;
	int mismatch = 1;
	int gap      = 2;

	int score;
	
	std::string larger;

	TNeedlemanWunsch *nw = new TNeedlemanWunsch(dnaa, dnab, match, mismatch, gap);

	struct timeval ti, tf;
	double time;

	gettimeofday(&ti, NULL);
		score = nw->FMakeMatrix();
	gettimeofday(&tf, NULL);
	time = (tf.tv_sec - ti.tv_sec)*1000 + (tf.tv_usec - ti.tv_usec)/1000;
	printf("time matrix:\t%.8lf s\n", time/1000);
	
	gettimeofday(&ti, NULL);
		nw->FGlobalOptimum(10);
	gettimeofday(&tf, NULL);
	time = (tf.tv_sec - ti.tv_sec)*1000 + (tf.tv_usec - ti.tv_usec)/1000;
	printf("time paths:\t%.8lf s\n\n", time/1000);
	
	larger = nw->FGetLargerSequence();
	// nw->FPrintBackMatrix();
	// nw->FPrintWeightMatrix();
	// std::vector<std::string> alignments = nw->FGetAllAlignment();
	std::vector<std::string> alignments = nw->FGetAlignment();

	std::cout << "larger: " << larger.size() << "\n";
	std::cout << "score:  " << score << "\n";
	std::cout << "paths:  " << ntotal << "\n";
	
	print_vector_r(alignments, larger);

	delete nw;
	return 0;
}