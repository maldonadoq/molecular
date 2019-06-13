#include <iostream>
#include <fstream>
#include <chrono>
#include <stdio.h>
#include "../utils/read.h"
#include "src/agglomerative.h"

using namespace std::chrono;

using std::cout;
using std::vector;
using std::ofstream;

template<class T>
class TC1{
public:
	inline T operator()(T x, T y){
		return std::min(x,y);
	}
};

template<class T>
class TC2{
public:
	inline T operator()(T x, T y){
		return std::max(x,y);
	}
};

template<class T>
class TC3{
public:
	inline T operator()(T x, T y){
		return (x+y)/2;
	}
};

typedef TAgglomerative<TC1<float> > ACS;	// Agglomerative Single Linkage
typedef TAgglomerative<TC2<float> > ACC;	// Agglomerative Complete Linkage
typedef TAgglomerative<TC3<float> > ACA;	// Agglomerative Average Linkage

typedef vector<float> dvect;			// String Vector

int main(int argc, char const *argv[]){
	vector<string> headers;
	vector<dvect>  distances;
	vector<string> clusters;

	ofstream file("../data/Maldonado-Result-Agg.dat");

	string filename = "../data/Cluster.txt";
	pre_processing_file(filename, distances, headers);

	unsigned ntypes = 3;
	unsigned ntest_size = 4;	
	int ntest_cluster[ntest_size];
	ntest_cluster[0] = 50;
	ntest_cluster[1] = 100;
	ntest_cluster[2] = 150;
	ntest_cluster[3] = 200;

	high_resolution_clock::time_point tinit;
	high_resolution_clock::time_point tend;
	duration<double> time_span;

	for(unsigned i=0; i<ntypes; i++){
		switch(i){
			case 0:{
				ACS *as = new ACS();
				file << "Single Linkage\n";
				cout << "Single Linkage\n";

				for(unsigned j=0; j<ntest_size; j++){
					as->Init(distances, headers);
					file << "N° Cluster: " << ntest_cluster[j] << "\n";
					cout << "  N° Cluster: " << ntest_cluster[j] << "\n";

					tinit = high_resolution_clock::now();
						as->Run(ntest_cluster[j]);
					tend = high_resolution_clock::now();
					time_span = duration_cast<duration<double>>(tend - tinit);
					file << "Time: " << time_span.count() << " s\n";
					cout << "   Time: " << time_span.count() << " s\n";

					clusters = as->GetClusters();
					for(unsigned k=0; k<clusters.size(); k++){
						file << "  [" << k << "]: " << clusters[k] << "\n";
					}
					file << "\n";
				}
				cout << "\n";
				delete as;
				break;
			}
			case 1:{
				ACC *ac = new ACC();
				file << "Complete Linkage\n";
				cout << "Complete Linkage\n";
				for(unsigned j=0; j<ntest_size; j++){
					ac->Init(distances, headers);
					file << "N° Cluster: " << ntest_cluster[j] << "\n";
					cout << "  N° Cluster: " << ntest_cluster[j] << "\n";

					tinit = high_resolution_clock::now();
						ac->Run(ntest_cluster[j]);
					tend = high_resolution_clock::now();
					time_span = duration_cast<duration<double>>(tend - tinit);
					file << "Time: " << time_span.count() << " s\n";
					cout << "   Time: " << time_span.count() << " s\n";

					clusters = ac->GetClusters();
					for(unsigned k=0; k<clusters.size(); k++){
						file << "  [" << k << "]: " << clusters[k] << "\n";
					}
					file << "\n";
				}
				cout << "\n";
				delete ac;
				break;
			}
			case 2:{
				ACA *aa = new ACA();
				file << "Average Linkage\n";
				cout << "Average Linkage\n";
				for(unsigned j=0; j<ntest_size; j++){
					aa->Init(distances, headers);
					file << "N° Cluster: " << ntest_cluster[j] << "\n";
					cout << "  N° Cluster: " << ntest_cluster[j] << "\n";

					tinit = high_resolution_clock::now();
						aa->Run(ntest_cluster[j]);
					tend = high_resolution_clock::now();
					time_span = duration_cast<duration<double>>(tend - tinit);
					file << "Time: " << time_span.count() << " s\n";
					cout << "   Time: " << time_span.count() << " s\n";

					clusters = aa->GetClusters();
					for(unsigned k=0; k<clusters.size(); k++){
						file << "  [" << k << "]: " << clusters[k] << "\n";
					}
					file << "\n";
				}
				cout << "\n";
				delete aa;
				break;
			}
		}
		file << "\n\n";
	}

	file.close();

	return 0;
}