#include <iostream>
#include <fstream>
#include <chrono>
#include <stdio.h>
#include "src/cmp.h"
#include "src/utils.h"
#include "src/agglomerative.h"

using namespace std::chrono;

typedef TAgglomerative<float, std::string, TC1<float> > ACS;	// Agglomerative Single Linkage
typedef TAgglomerative<float, std::string, TC2<float> > ACC;	// Agglomerative Complete Linkage
typedef TAgglomerative<float, std::string, TC3<float> > ACA;	// Agglomerative Average Linkage

typedef std::vector<float> dvect;								// String Vector

int main(int argc, char const *argv[]){
	std::vector<std::string > headers;
	std::vector<dvect>        distances;
	std::vector<std::string > clusters;

	std::ofstream file("../data/Maldonado-Result-Agg.dat");

	std::string filename = "../data/Diauxic.txt";
	pre_processing_file(filename, distances, headers);

	unsigned ntypes = 3;
	unsigned ntest_size = 4;	
	int ntest_cluster[ntest_size] = {50, 100, 150, 200};

	high_resolution_clock::time_point tinit;
	high_resolution_clock::time_point tend;
	duration<double> time_span;

	for(unsigned i=0; i<ntypes; i++){
		switch(i){
			case 0:{
				ACS *as = new ACS();
				file << "Single Linkage\n";

				for(unsigned j=0; j<ntest_size; j++){
					as->Init(distances, headers);
					file << " N° Cluster: " << ntest_cluster[j] << "\n";

					tinit = high_resolution_clock::now();
						as->Run(ntest_cluster[j]);
					tend = high_resolution_clock::now();
					time_span = duration_cast<duration<double>>(tend - tinit);
					file << " Time: " << time_span.count() << " s\n";

					clusters = as->GetClusters();
					for(unsigned k=0; k<clusters.size(); k++){
						file << "  [" << k << "]: " << clusters[k] << "\n";
					}
					file << "\n";
				}
				delete as;
				break;
			}
			case 1:{
				ACC *ac = new ACC();
				file << "Complete Linkage\n";
				for(unsigned j=0; j<ntest_size; j++){
					ac->Init(distances, headers);
					file << " N° Cluster: " << ntest_cluster[j] << "\n";

					tinit = high_resolution_clock::now();
						ac->Run(ntest_cluster[j]);
					tend = high_resolution_clock::now();
					time_span = duration_cast<duration<double>>(tend - tinit);
					file << " Time: " << time_span.count() << " s\n";

					clusters = ac->GetClusters();
					for(unsigned k=0; k<clusters.size(); k++){
						file << "  [" << k << "]: " << clusters[k] << "\n";
					}
					file << "\n";
				}
				delete ac;
				break;
			}
			case 2:{
				ACA *aa = new ACA();
				file << "Average Linkage\n";
				for(unsigned j=0; j<ntest_size; j++){
					aa->Init(distances, headers);
					file << " N° Cluster: " << ntest_cluster[j] << "\n";

					tinit = high_resolution_clock::now();
						aa->Run(ntest_cluster[j]);
					tend = high_resolution_clock::now();
					time_span = duration_cast<duration<double>>(tend - tinit);
					file << " Time: " << time_span.count() << " s\n";

					clusters = aa->GetClusters();
					for(unsigned k=0; k<clusters.size(); k++){
						file << "  [" << k << "]: " << clusters[k] << "\n";
					}
					file << "\n";
				}
				delete aa;
				break;
			}
		}
		file << "\n\n";
	}

	file.close();

	return 0;
}