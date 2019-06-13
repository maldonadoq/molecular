#include <iostream>
#include <stdio.h>
#include <chrono>
#include "../utils/read.h"
#include "src/upgma.h"

using namespace std::chrono;

using std::string;
using std::cout;
using std::stoi;

typedef std::vector<float> dvect;

// build: g++ upgma.cpp -o upgma.out
int main(int argc, char const *argv[]){
	vector<string> headers;
	vector<dvect>  distances;

	string filename = "../data/Upgma-Small.txt";
	pre_processing_file(filename, distances, headers);

	high_resolution_clock::time_point tinit;
	high_resolution_clock::time_point tend;
	duration<double> time_span;	

	unsigned ncluster = 1;

	TUpgma *up = new TUpgma();
	up->Init(distances, headers);

	tinit = high_resolution_clock::now();
		up->Run(ncluster);
	tend = high_resolution_clock::now();
	time_span = duration_cast<duration<double>>(tend - tinit);
	std::cout << "[cluster time] : " << time_span.count() << " s\n";

	system("python upgma.py");

	delete up;
	return 0;
}