#include <iostream>
#include <thread>
#include <vector>
#include <utility>
#include <chrono>
#include "src/utils.h"

using namespace std::chrono;

using std::cout;
using std::vector;
using std::string;
using std::pair;
using std::thread;
using std::make_pair;

vector<vector<float> >	distances;
vector<pair<int,int> >	minimuns;

void FindMinNormal(){
	int mini = 1;
	int minj = 0;

	unsigned i,j;
	for(i=2; i<distances.size(); i++){
		for(j=0; j<i; j++){
			if(distances[i][j] < distances[mini][minj]){
				mini = i;
				minj = j;
			}
		}
	}

	cout << "min: [" << mini << "," << minj << "]\n";
}

void FindMin(int _idx){
	int i;
	int mini = _idx;
	int minj = 0;

	for(i=1; i<_idx; i++){
		if(distances[_idx][i] < distances[_idx][minj]){
			minj = i;
		}
	}

	minimuns[i-2] = make_pair(mini, minj);
}

void FindMinParallel1(){
	int mini = 1;
	int minj = 0;

	int nt = distances.size()-2;
	thread th[nt];

	minimuns.resize(nt);

	int i;
	for(i=0; i<nt; i++){
		th[i] = thread(FindMin, (int)i+2);
	}

	for(i=0; i<nt; i++){
		th[i].join();
	}

	pair<int, int> tmp;
	for(i=0; i<(int)minimuns.size(); i++){
		tmp = minimuns[i];
		if(distances[tmp.first][tmp.second] < distances[mini][minj]){
			mini = tmp.first;
			minj = tmp.second;
		}
	}

	cout << "min: [" << mini << "," << minj << "]\n";
}

void FindMinParallel2(){
	int mini = 1;
	int minj = 0;

	int number_rows = distances.size()-2;
	minimuns.resize(number_rows);

	int th  = thread::hardware_concurrency();
	int np  = number_rows/th;
	int res = number_rows%th;

	thread tthread[th];

	int i, j;
	for(i=0; i<(th*np); i+=th){
		for(j=0; j<th; j++){
			tthread[j] = thread(FindMin, i+j+2);
			// cout << i+j+2 << " ";
		}

		for(j=0; j<th; j++){
			tthread[j].join();
		}
	}
	if(res > 0){
		for(j=0; j<res; j++){
			// cout << (th*np)+j+2 << " ";
			tthread[j] = thread(FindMin, (th*np)+j+2);
		}

		for(j=0; j<res; j++){
			tthread[j].join();
		}
	}

	pair<int, int> tmp;
	for(i=0; i<(int)minimuns.size(); i++){
		tmp = minimuns[i];
		if(distances[tmp.first][tmp.second] < distances[mini][minj]){
			mini = tmp.first;
			minj = tmp.second;
		}
	}

	cout << "min: [" << mini << "," << minj << "]\n";
}

void create_matrix(int _t){
	distances = std::vector<vector<float> >(_t, vector<float>(_t));
	float tmp;
	for(int i=0; i<_t; i++){
		for(int j=0; j<i; j++){
			tmp = (float)rand()/(float)RAND_MAX;

			distances[i][j] = tmp;
			distances[j][i] = tmp;
		}

		distances[i][i] = 0;
	}
}

int main(int argc, char const *argv[]){
	create_matrix(832);

	high_resolution_clock::time_point tinit;
	high_resolution_clock::time_point tend;
	duration<double> time_span;

	tinit = high_resolution_clock::now();
		FindMinNormal();
	tend = high_resolution_clock::now();	
	time_span = duration_cast<duration<double>>(tend - tinit);
	std::cout << "[normal] : " << time_span.count() << " s\n";	

	tinit = high_resolution_clock::now();
		FindMinParallel1();
	tend = high_resolution_clock::now();	
	time_span = duration_cast<duration<double>>(tend - tinit);
	std::cout << "[paral1] : " << time_span.count() << " s\n";	

	tinit = high_resolution_clock::now();
		FindMinParallel2();
	tend = high_resolution_clock::now();	
	time_span = duration_cast<duration<double>>(tend - tinit);
	std::cout << "[paral2] : " << time_span.count() << " s\n";	

	return 0;
}