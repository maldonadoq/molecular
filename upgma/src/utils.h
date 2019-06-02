#ifndef _UTILS_H_
#define _UTILS_H_

#include <vector>
#include <iostream>
#include <fstream>
#include <math.h>

using std::cout;
using std::vector;
using std::string;
using std::ifstream;

template<class T>
void print_vector(vector<T> _vector){
	for(unsigned i=0; i<_vector.size(); i++){
		cout << "[" << _vector[i] << "] ";
	}
	cout << "\n";
}

void pre_processing_file(string _filename,
	vector<vector<float> > &_vdata, vector<string> &_vname){

	ifstream file(_filename);

	if(!file.is_open()){
		cout << "error! cvs wrong!!\n";
		return;
	}
	
	vector<vector<float> > vdata;
	vector<float> tmp(7);
	string value;

	getline(file,value,'\n');			//RowName
	while(file.good()){		
		getline(file,value,'\t');
		_vname.push_back(value);

		getline(file,value,'\t');	tmp[0] = stod(value); // X1
		getline(file,value,'\t');	tmp[1] = stod(value); // X2
		getline(file,value,'\t');	tmp[2] = stod(value); // X3
		getline(file,value,'\t');	tmp[3] = stod(value); // X4
		getline(file,value,'\t');	tmp[4] = stod(value); // X5
		getline(file,value,'\t');	tmp[5] = stod(value); // X6
		getline(file,value,'\n');	tmp[6] = stod(value); // X7

		vdata.push_back(tmp);
		// print_vector(tmp);
	}

	cout << "data: " << vdata.size() << "\n";
	cout << "name: " << _vname.size() << "\n";

	file.close();

	unsigned ndim = 7;
	float dist;

	for(unsigned i=0; i<vdata.size(); i++){
		tmp.clear();		
		for(unsigned j=0; j<i+1; j++){
			dist = 0;
			for(unsigned k=0; k<ndim; k++){
				dist += (pow(vdata[i][k] - vdata[j][k],2));
			}
			tmp.push_back(sqrt(dist));
		}
		_vdata.push_back(tmp);
	}
}

#endif