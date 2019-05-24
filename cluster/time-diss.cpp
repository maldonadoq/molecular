#include <iostream>
#include <fstream>
#include <chrono>
#include <stdio.h>
#include "src/cmp.h"
#include "src/utils.h"
#include "src/dissociative.h"

using namespace std::chrono;

typedef TDissociative<float, std::string> DC;					// Dissociative Cluster
typedef std::vector<float> dvect;								// String Vector

// build: g++ time-diss.cpp -o time-diss.out
int main(int argc, char const *argv[]){
	std::vector<std::string > headers;
	std::vector<dvect>        distances;
	std::vector<std::string > clusters;

	std::ofstream file("../data/Maldonado-Result-Diss.dat");

	std::string filename = "../data/Diauxic.txt";
	pre_processing_file(filename, distances, headers);

	unsigned ntypes = 3;
	std::vector<std::string> ntypes_name = {"Single Linkage\n","Complete Linkage\n","Average Linkage\n"};

	unsigned ntest_size = 4;	
	int ntest_cluster[ntest_size] = {50, 100, 150, 200};

	high_resolution_clock::time_point tinit;
	high_resolution_clock::time_point tend;
	duration<double> time_span;

	for(unsigned i=0; i<ntypes; i++){
		file << ntypes_name[i];
		DC *diss = new DC();
		for(unsigned j=0; j<ntest_size; j++){
			diss->Init(distances, headers);
			diss->SetType(i);
			file << " N° Cluster: " << ntest_cluster[j] << "\n";

			tinit = high_resolution_clock::now();
				diss->Run(ntest_cluster[j]);
			tend = high_resolution_clock::now();
			time_span = duration_cast<duration<double>>(tend - tinit);

			clusters = diss->GetClusters();
			for(unsigned k=0; k<clusters.size(); k++){
				file << "  [" << k << "]: " << clusters[k] << "\n";
			}
			file << "\n";
		}
		delete diss;
		file << "\n\n";
	}

	file.close();

	return 0;
}