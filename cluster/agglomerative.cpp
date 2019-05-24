#include <iostream>
#include <chrono>
#include <stdio.h>
#include "src/cmp.h"
#include "src/utils.h"
#include "src/agglomerative.h"

using namespace std::chrono;

typedef TAgglomerative<float, std::string, TC<float> > Cluster;
typedef std::vector<float> dvect;

// build: g++ agglomerative.cpp -o agglomerative.out
int main(int argc, char const *argv[]){
	int nc = 7;
	std::vector<std::string > headers   = {"A","B","C","D","E","F","G"};
	std::vector<dvect>        distances = std::vector<dvect>(nc, dvect(nc));
	std::vector<std::string > clusters;

	distances[0] = {0.00};
	distances[1] = {2.15, 0.00};
	distances[2] = {0.70, 1.53, 0.00};
	distances[3] = {1.07, 1.14, 0.43, 0.00};
	distances[4] = {0.85, 1.38, 0.21, 0.29, 0.00};
	distances[5] = {1.16, 1.01, 0.55, 0.22, 0.41, 0.00};
	distances[6] = {1.56, 2.83, 1.86, 2.04, 2.02, 2.05, 0.00};

	unsigned ncluster = 3;

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
	print_vector(clusters);

	delete cl;

	return 0;
}