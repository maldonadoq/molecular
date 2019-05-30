#include <iostream>
#include <chrono>
#include <stdio.h>
#include "src/cmp.h"
#include "src/utils.h"
#include "src/tagglomerative.h"

using namespace std::chrono;

typedef TAgglomerative<TC<float> > Cluster;
typedef std::vector<float> dvect;

// build: g++ agglomerative.cpp -o agglomerative.out -pthread -O1
int main(int argc, char const *argv[]){
	std::vector<std::string > headers;
	std::vector<dvect>        distances;
	std::vector<std::string > clusters;

	std::string filename = "../data/Diauxic.txt";
	pre_processing_file(filename, distances, headers);

	unsigned ncluster = 50;

	Cluster *cl = new Cluster();

	high_resolution_clock::time_point tinit;
	high_resolution_clock::time_point tend;
	duration<double> time_span;

	cl->Init(distances, headers);	

	tinit = high_resolution_clock::now();
		cl->Run(ncluster);
	tend = high_resolution_clock::now();
	time_span = duration_cast<duration<double>>(tend - tinit);
	std::cout << "[cluster time] : " << time_span.count() << " s\n";	

	clusters = cl->GetClusters();
	// print_vector(clusters);

	delete cl;

	return 0;
}