#include <iostream>
#include <chrono>
#include <stdio.h>
#include "src/utils.h"
#include "src/upgma.h"

using namespace std::chrono;

using std::string;
using std::cout;

typedef std::vector<float> dvect;

// build: g++ upgma.cpp -o upgma.out
int main(int argc, char const *argv[]){
	vector<string> headers;
	vector<dvect>  distances;

	string filename = "../data/Upgma.txt";
	pre_processing_file(filename, distances, headers);

	unsigned ncluster = 5;

	TUpgma *up = new TUpgma();

	high_resolution_clock::time_point tinit;
	high_resolution_clock::time_point tend;
	duration<double> time_span;

	up->Init(distances, headers);	

	tinit = high_resolution_clock::now();
		up->Run(ncluster);
	tend = high_resolution_clock::now();
	time_span = duration_cast<duration<double>>(tend - tinit);
	std::cout << "[cluster time] : " << time_span.count() << " s\n";

	delete up;
	return 0;
}