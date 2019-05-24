#ifndef _UTILS_H_
#define _UTILS_H_

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>

template<class T>
void print_vector(std::vector<T> _v){
	for(unsigned i=0; i<_v.size(); i++){
		std::cout << "[" << _v[i] << "]  ";
	}
	std::cout << "\n";
}

void pre_processing_file(std::string _filename,
	std::vector<std::vector<float> > &_vdata, std::vector<std::string> &_vname){

	std::ifstream file(_filename);

	if(!file.is_open()){
		std::cout << "error! cvs wrong!!\n";
		return;
	}
	
	std::vector<std::vector<float> > vdata;
	std::vector<float> tmp(7);
	std::string value;

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

	std::cout << "data: " << vdata.size() << "\n";
	std::cout << "name: " << _vname.size() << "\n";

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
		// print_vector(tmp);

		_vdata.push_back(tmp);
	}
}

#endif