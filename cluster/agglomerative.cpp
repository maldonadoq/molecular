#include <iostream>
#include <chrono>
#include <stdio.h>
#include "src/utils.h"
#include "src/agglomerative.h"

using namespace std::chrono;

template<class T>
class TC{
public:
	inline T operator()(T x, T y){
		// return std::min(x,y);
		// return std::max(x,y);
		return (x+y)/2;
	}
};

typedef TAgglomerative<float, std::string, TC<float> > Cluster;
typedef std::vector<float> dvect;

int main(int argc, char const *argv[]){
	std::vector<std::string > headers;
	std::vector<dvect>        distances;

	std::string filename = "../data/Diauxic.txt";
	pre_processing_file(filename, distances, headers);

	/*int nc = 7;
	std::vector<std::string > headers   = {"A","B","C","D","E","F","G"};
	std::vector<dvect>        distances = std::vector<dvect>(nc, dvect(nc));

	distances[0] = {0.00};
	distances[1] = {2.15, 0.00};
	distances[2] = {0.70, 1.53, 0.00};
	distances[3] = {1.07, 1.14, 0.43, 0.00};
	distances[4] = {0.85, 1.38, 0.21, 0.29, 0.00};
	distances[5] = {1.16, 1.01, 0.55, 0.22, 0.41, 0.00};
	distances[6] = {1.56, 2.83, 1.86, 2.04, 2.02, 2.05, 0.00};*/

	unsigned ncluster = 250;

	// std::cout << "cluster's number: "; std::cin >> ncluster;

	Cluster *cl = new Cluster();

	high_resolution_clock::time_point tinit;
	high_resolution_clock::time_point tend;
	duration<double> time_span;

	cl->Init(distances, headers);	

	tinit = high_resolution_clock::now();
		cl->Cluster(ncluster);
	tend = high_resolution_clock::now();
	time_span = duration_cast<duration<double>>(tend - tinit);
	std::cout << "[cluster time] : " << time_span.count() << " s\n";	

	delete cl;

	return 0;
}